/**
 * \file allocator_traits.hpp
 *
 * \brief This header contains the trait type \c allocator_traits that
 *        determines or infers default values for the Allocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP
#define BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP

#include "detail/allocator_function_traits.hpp"

#include "../allocator_info.hpp" // allocator_info
#include "../errors.hpp"         // get_out_of_memory_handler

#include <type_traits> // std::true_type, std::false_type, etc
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <limits>      // std::numeric_limits
#include <memory>      // std::addressof

namespace bit {
  namespace memory {

    /// \brief Type-trait to determine whether \p T is an allocator
    ///
    /// The result is \c std::true_type if \p T satisfies the Allocator concept,
    /// which minimally requires the following to be well formed:
    ///
    /// \code
    /// std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>() );
    /// std::declval<T&>().deallocate( std::declval<void*>(), std::declval<std::size_t>() );
    /// \endcode
    ///
    /// The result is accessible as \c ::value
    template<typename T>
    using is_allocator = detail::is_allocator<T>;

    /// \brief Convenience template variable to extract whether \p T is an
    ///        allocator
    template<typename T>
    constexpr bool is_allocator_v = is_allocator<T>::value;


    /// \brief Type-trait to determine whether \p T is an extended allocator
    ///
    /// The result is \c std::true_type if \p T satisfies the ExtendedAllocator
    /// concept, which requires the following:
    ///
    /// \c T must satisfy \c Allocator, and the expressions must be
    /// well-formed:
    ///
    /// \code
    /// std::declval<T&>().try_allocate( std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<std::size_t>() );
    /// std::declval<T&>().owns( std::declval<void*>() );
    /// \endcode
    ///
    /// The result is accessible as \c ::value
    template<typename T>
    using is_extended_allocator = detail::is_allocator<T>;

    /// \brief Convenience template variable to extract whether \p T is an
    ///        extended allocator
    template<typename T>
    constexpr bool is_extended_allocator_v = is_extended_allocator<T>::value;

    //////////////////////////////////////////////////////////////////////////
    /// \brief The allocator_traits class template provides a standardized
    ///        way to access allocator functionality
    ///
    /// This will infer defaults for any optional values that may not be
    /// present in the allocator
    ///
    /// \tparam Allocator the allocator type. Must satisfy Allocator concept
    //////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_traits
    {

      static_assert( is_allocator<Allocator>::value, "Allocator must be an Allocator" );

      //----------------------------------------------------------------------
      // Public Members
      //----------------------------------------------------------------------
    public:

      using is_always_equal   = detail::allocator_is_always_equal<Allocator>;
      using is_stateless      = detail::allocator_is_stateless<Allocator>;
      using default_alignment = detail::allocator_default_alignment<Allocator>;
      using max_alignment     = detail::allocator_max_alignment<Allocator>;
      using can_truncate_deallocations = detail::allocator_can_truncate_deallocations<Allocator>;
      using knows_ownership   = detail::allocator_has_owns<Allocator>;

      //----------------------------------------------------------------------
      // Allocation
      //----------------------------------------------------------------------
    public:

      /// \brief Attempts to allocate memory of at least \p size bytes,
      ///        aligned to \p align boundary
      ///
      /// On failure, this function returns \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static void* try_allocate( Allocator& alloc,
                                 std::size_t size,
                                 std::size_t align ) noexcept;

      /// \brief Allocates memory of at least \p size bytes, aligned to \p
      ///        align boundary
      ///
      /// On failure, this function may throw or invoke the out_of_memory
      /// handler before returning \p nullptr
      ///
      /// \param alloc the allocator to allocate from
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the pointer to the allocated member
      static void* allocate( Allocator& alloc,
                             std::size_t size,
                             std::size_t align );

      //----------------------------------------------------------------------
      // Deallocation
      //----------------------------------------------------------------------

      /// \brief Deallocates a pointer previously allocated with \p allocate
      ///        or try_allocate
      ///
      /// \param alloc the allocator to allocate from
      /// \param p the pointer to deallocate
      /// \param size the size of the allocation
      static void deallocate( Allocator& alloc, void* p, std::size_t size );

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------

      /// \brief Checks if the given allocator is known to own the specified
      ///        pointer \p p
      ///
      /// \note This directly invokes \c alloc.owns(p) . It is undefined
      ///       behaviour to invoke this if \c Allocator::owns is not
      ///       defined; branches should instead be taken by using
      ///       \ref knows_ownership
      ///
      /// \param alloc the allocator
      /// \param p the pointer
      static bool owns( Allocator& alloc, const void* p ) noexcept;

      //----------------------------------------------------------------------
      // Capacity
      //----------------------------------------------------------------------

      /// \brief Gets the maximum size allocateable from this allocator
      ///
      /// \note the max_size does not guarantee that the size is able to be
      ///       allocated at the time the value is queried; it just means that
      ///       it is the maximum size that the allocator is capable of
      ///       allocating from an empty state.
      ///
      /// \param alloc the allocator to get the max size from
      /// \return the amount of bytes available for the largest possible
      ///         allocation
      static std::size_t max_size( const Allocator& alloc ) noexcept;

      /// \brief Gets the minimum size allocateable from this allocator
      ///
      /// \note The default is 1, if this function is not defined
      ///
      /// \param alloc the allocator to get the min size from
      /// \return the minimum amount of bytes able to allocated
      static std::size_t min_size( const Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Name
      //----------------------------------------------------------------------

      /// \brief Gets the name of the specified allocator
      ///
      /// \note Not all allocators are nameable or have a name specified.
      ///       For these allocators, the string returned is "Unnamed"
      ///
      /// \note The lifetime of the pointer returned is unmanaged, and is NOT
      ///       the responsibility of the caller to free.
      ///
      /// \param alloc the allocator to get the name of
      /// \return the name of the allocator
      static allocator_info info( const Allocator& alloc ) noexcept;

      //----------------------------------------------------------------------
      // Private Implementation
      //----------------------------------------------------------------------
    private:

      /// \{
      /// \brief Performs the allocation, either by calling
      ///        \c Allocator::allocate, or by calling into
      ///        \c Allocator::try_allocate and defining a boilerplate
      ///        allocate function
      ///
      /// \param alloc the allocator to allocate
      /// \param size the size of the allocation
      /// \param align the alignment of the allocation
      /// \return the allocated pointer
      static void* do_allocate( std::true_type, Allocator& alloc, std::size_t size, std::size_t align );
      static void* do_allocate( std::false_type, Allocator& alloc, std::size_t size, std::size_t align );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the max size of the allocator, either by
      ///        calling \c Allocator::max_size, or by assuming
      ///        \c std::numeric_limits<std::size_t>::max()
      ///
      /// \param alloc the allocator
      /// \return the max size
      static std::size_t do_max_size( std::true_type, const Allocator& alloc );
      static std::size_t do_max_size( std::false_type, const Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the min size of an allocation, either by
      ///        calling \c Allocator::min_size(), or by assuming
      ///        \c 1
      ///
      /// \param alloc the allocator
      /// \return the max size
      static std::size_t do_min_size( std::true_type, const Allocator& alloc );
      static std::size_t do_min_size( std::false_type, const Allocator& alloc );
      /// \}

      //----------------------------------------------------------------------

      /// \{
      /// \brief Determines the info for the allocator, either by calling
      ///        \c Allocator::info or by assuming "Unnamed" for the name
      ///
      /// \param alloc the allocator
      /// \return the name of the allocator
      static allocator_info do_info( std::true_type, const Allocator& alloc );
      static allocator_info do_info( std::false_type, const Allocator& alloc );
      /// \}

    };

  } // namespace memory
} // namespace bit

#include "detail/allocator_traits.inl"

#endif /* BIT_MEMORY_ALLOCATORS_ALLOCATOR_TRAITS_HPP */

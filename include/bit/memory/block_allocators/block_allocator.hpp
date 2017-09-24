/**
 * \file block_allocator.hpp
 *
 * \brief This header contains a type-erased view of a BlockAllocator concept
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#ifndef BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_HPP
#define BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_HPP

#include "../memory.hpp"
#include "../memory_block.hpp"
#include "../concepts.hpp"

namespace bit {
  namespace memory {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A type erased view of allocators that satisfy the BlockAllocator
    ///        concept.
    ///
    /// This view is non-owning, and thus does not participate in the lifetime
    /// of a given allocator.
    ///
    /// \satisfies BlockAllocator
    //////////////////////////////////////////////////////////////////////////
    class block_allocator final
    {
      template<typename A>
      using is_enabled = std::integral_constant<bool,
        is_block_allocator<A>::value && !std::is_same<block_allocator,A>::value
      >;

      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a block allocator by type erasing the underlying
      ///        allocator
      ///
      /// \param allocator the block allocator to type-erase
      template<typename BlockAllocator, typename = std::enable_if_t<is_enabled<BlockAllocator>::value>>
      block_allocator( BlockAllocator&& allocator ) noexcept;

      /// \brief Copy-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      block_allocator( const block_allocator& other ) noexcept = default;

      /// \brief Move-constructs a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      block_allocator( block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------

      /// \brief Copy-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to copy
      /// \return reference to \c (*this)
      block_allocator& operator = ( const block_allocator& other ) noexcept = default;

      /// \brief Move-assigns a block_allocator from an existing one
      ///
      /// \param other the other allocator to move
      /// \return reference to \c (*this)
      block_allocator& operator = ( block_allocator&& other ) noexcept = default;

      //----------------------------------------------------------------------
      // Block Allocations
      //----------------------------------------------------------------------
    public:

      /// \brief Allocates a block from the underlying block allocator
      ///
      /// \return the allocated memory block
      owner<memory_block> allocate_block();

      /// \brief Deallocates a block from the underlying block allocatore
      ///
      /// \param block the block to deallocate
      void deallocate_block( owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      using deallocate_block_fn_t = void(*)( void*, memory_block );
      using allocate_block_fn_t   = memory_block(*)( void* );

      //----------------------------------------------------------------------
      // Private Static Functions
      //----------------------------------------------------------------------
    private:

      template<typename BlockAllocator>
      static owner<memory_block> allocate_block_function( void* instance );

      template<typename BlockAllocator>
      static void deallocate_block_function( void* instance, owner<memory_block> block );

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      void*                 m_ptr;
      allocate_block_fn_t   m_allocate_block_fn;
      deallocate_block_fn_t m_deallocate_block_fn;

    };

  } // namespace memory
} // namespace bit

#include "detail/block_allocator.inl"

#endif /* BIT_MEMORY_BLOCK_ALLOCATORS_BLOCK_ALLOCATOR_HPP */

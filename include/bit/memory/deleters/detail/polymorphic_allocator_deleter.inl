#ifndef BIT_MEMORY_DELETERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL
#define BIT_MEMORY_DELETERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL

//=============================================================================
// polymorphic_allocator_deleter<T,AllocatorStorage>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( AllocatorStorage storage )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storage) ) ),
    m_size( sizeof(T) )
{

}

//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( polymorphic_allocator_deleter<U,AllocatorStorage>&& other )
  noexcept
  : base_type( std::forward_as_tuple( other.get_storage() ) ),
    m_size( other.size() )
{

}

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::polymorphic_allocator_deleter( const polymorphic_allocator_deleter<U,AllocatorStorage>& other )
  noexcept
  : base_type( std::forward_as_tuple( other.get_storage() ) ),
    m_size( other.size() )
{

}

//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>&
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator=( polymorphic_allocator_deleter<U,AllocatorStorage>&& other )
  noexcept
{
  detail::get<0>(*this) = other.storage();
  m_size = other.size();

  return (*this);
}

template<typename T, typename AllocatorStorage>
template<typename U, typename>
inline bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>&
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator=( const polymorphic_allocator_deleter<U,AllocatorStorage>& other )
  noexcept
{
  detail::get<0>(*this) = other.storage();
  m_size = other.size();

  return (*this);
}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline void bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>
  ::operator()( pointer p )
{
  auto& storage   = detail::get<0>(*this);
  auto& allocator = storage.get_allocator();

  destroy_at( to_raw_pointer(p) );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), m_size );
}

//-----------------------------------------------------------------------------
// Private Observers
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline AllocatorStorage
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>::storage()
  const noexcept
{
  return detail::get<0>(*this);
}

template<typename T, typename AllocatorStorage>
inline std::size_t
  bit::memory::polymorphic_allocator_deleter<T,AllocatorStorage>::size()
  const noexcept
{
  return m_size;
}

//=============================================================================
// polymorphic_allocator_deleter<T,Allocator>
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline bit::memory::polymorphic_allocator_deleter<T[],AllocatorStorage>
  ::polymorphic_allocator_deleter( AllocatorStorage storage, size_type size )
  noexcept
  : base_type( std::forward_as_tuple( std::move(storage) ) ),
    m_size( size )
{

}

//-----------------------------------------------------------------------------
// Deallocations
//-----------------------------------------------------------------------------

template<typename T, typename AllocatorStorage>
inline void bit::memory::polymorphic_allocator_deleter<T[],AllocatorStorage>
  ::operator()( pointer p )
{
  auto& storage   = detail::get<0>(*this);
  auto& allocator = storage.get_allocator();

  destroy_array_at( to_raw_pointer(p), m_size );
  alloc_traits::deallocate( allocator, static_cast<void_pointer>(p), sizeof(T) * m_size );
}

#endif /* BIT_MEMORY_DELETERS_DETAIL_POLYMORPHIC_ALLOCATOR_DELETER_INL */
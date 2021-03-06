#ifndef BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_ALLOCATOR_INL
#define BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_ALLOCATOR_INL

//============================================================================
// bump_up_allocator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

inline bit::memory::bump_up_allocator::bump_up_allocator( memory_block block )
  noexcept
  : m_block(block),
    m_current(m_block.data())
{
  assert( m_block && "Block must not be null" );
}

//----------------------------------------------------------------------------
// Allocation / Deallocation
//----------------------------------------------------------------------------

inline bit::memory::owner<void*>
  bit::memory::bump_up_allocator::try_allocate( std::size_t size,
                                                std::size_t align,
                                                std::size_t offset )
  noexcept
{
  assert( size && "cannot allocate 0 bytes");
  assert( align && "cannot allocate with 0 alignment");
  assert( is_power_of_two(align) && "alignment must be a power of two" );

  using byte_t = unsigned char;

  auto* p = offset_align_forward(m_current,align,offset);

  auto* p_end = static_cast<byte_t*>(p) + size;

  // If allocated outside the range, return nullptr
  if( BIT_MEMORY_UNLIKELY( p_end > m_block.end_address() ) )
    return nullptr;

  // bump the pointer
  m_current = p_end;

  return p;
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_up_allocator::deallocate( owner<void*> p,
                                                        std::size_t size )
{
  BIT_MEMORY_UNUSED(p);
  BIT_MEMORY_UNUSED(size);

  assert( m_block.contains( p ) && "Pointer must be contained by block" );

  // bump_up_allocator only uses truncated deallocations with deallocate_all
}

//----------------------------------------------------------------------------

inline void bit::memory::bump_up_allocator::deallocate_all()
  noexcept
{
  m_current = m_block.data();
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

inline bool bit::memory::bump_up_allocator::owns( const void* p )
  const noexcept
{
  return m_block.start_address() <= p && p < m_current;
}

inline bit::memory::allocator_info bit::memory::bump_up_allocator::info()
  const noexcept
{
  return {"bump_up_allocator",this};
}

#endif /* BIT_MEMORY_ALLOCATORS_DETAIL_BUMP_UP_ALLOCATOR_INL */

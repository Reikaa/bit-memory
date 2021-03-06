cmake_minimum_required(VERSION 3.1)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

include(AddHeaderSelfContainmentTest)
include(MakeVersionHeader)
include(CopyTargetProperties)
include(GroupSourceTree)

#-----------------------------------------------------------------------------
# Project Setup
#-----------------------------------------------------------------------------

option(BIT_MEMORY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS "Include each header independently in a .cpp file to determine header self-containment" off)
option(BIT_MEMORY_COMPILE_UNIT_TESTS "Compile and run the unit tests for this library" off)
option(BIT_MEMORY_GENERATE_DOCUMENTATION "Generates doxygen documentation" off)
option(BIT_MEMORY_COMPILE_BENCHMARKS "Compile the benchmark tests for this library" off)
option(BIT_MEMORY_VERBOSE_CONFIGURE "Configures the CMakeLists.txt with logging messages" off)

project("BitMemory")

if( CMAKE_BUILD_TYPE STREQUAL "" )
  if( BIT_MEMORY_VERBOSE_CONFIGURE )
    message(STATUS "CMAKE_BUILD_TYPE not set; defaulting to 'Debug'")
  endif()
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

#-----------------------------------------------------------------------------
# Version Info
#-----------------------------------------------------------------------------

set(BIT_MEMORY_VERSION_MAJOR 0 CACHE STRING "major version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_MINOR 1 CACHE STRING "minor version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION_PATCH 188 CACHE STRING "patch version of bit::memory" FORCE)
set(BIT_MEMORY_VERSION "${BIT_MEMORY_VERSION_MAJOR}.${BIT_MEMORY_VERSION_MINOR}.${BIT_MEMORY_VERSION_PATCH}" CACHE STRING "version of bit::memory" FORCE)

if( BIT_MEMORY_VERBOSE_CONFIGURE )
  message(STATUS "bit::memory ${BIT_MEMORY_VERSION}")
endif()

#-----------------------------------------------------------------------------
# bit::memory: Sources
#-----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

make_version_header("${CMAKE_CURRENT_BINARY_DIR}/include/bit/memory/version.hpp"
  MAJOR ${BIT_MEMORY_VERSION_MAJOR}
  MINOR ${BIT_MEMORY_VERSION_MINOR}
  PATCH ${BIT_MEMORY_VERSION_PATCH}
  PREFIX BIT_MEMORY
  SUFFIX "a"
)

set(headers
  # Utilities
  include/bit/memory/utilities/address.hpp
  include/bit/memory/utilities/allocator_info.hpp
  include/bit/memory/utilities/debugging.hpp
  include/bit/memory/utilities/dynamic_size_type.hpp
  include/bit/memory/utilities/ebo_storage.hpp
  include/bit/memory/utilities/endian.hpp
  include/bit/memory/utilities/errors.hpp
  include/bit/memory/utilities/freelist.hpp
  include/bit/memory/utilities/macros.hpp
  include/bit/memory/utilities/memory_block.hpp
  include/bit/memory/utilities/memory_block_cache.hpp
  include/bit/memory/utilities/not_null.hpp
  include/bit/memory/utilities/owner.hpp
  include/bit/memory/utilities/pointer_utilities.hpp
  include/bit/memory/utilities/unaligned_storage.hpp
  include/bit/memory/utilities/uninitialized_storage.hpp

  # Concepts
  include/bit/memory/concepts/detail/void_t.hpp
  include/bit/memory/concepts/detail/identity.hpp
  include/bit/memory/concepts/Stateless.hpp
  include/bit/memory/concepts/Allocator.hpp
  include/bit/memory/concepts/AllocatorStorage.hpp
  include/bit/memory/concepts/BlockAllocator.hpp
  include/bit/memory/concepts/BlockAllocatorStorage.hpp
  include/bit/memory/concepts/BasicLockable.hpp
  include/bit/memory/concepts/BoundsChecker.hpp
  include/bit/memory/concepts/Deleter.hpp
  include/bit/memory/concepts/ExtendedAllocator.hpp
  include/bit/memory/concepts/MemoryTagger.hpp
  include/bit/memory/concepts/MemoryTracker.hpp

  # Traits
  include/bit/memory/traits/allocator_traits.hpp
  include/bit/memory/traits/block_allocator_traits.hpp
  include/bit/memory/traits/extended_allocator_traits.hpp

  # Regions
  include/bit/memory/regions/aligned_heap_memory.hpp
  include/bit/memory/regions/virtual_memory.hpp

  # Adapters
  include/bit/memory/adapters/allocator_deleter.hpp
  include/bit/memory/adapters/polymorphic_allocator_deleter.hpp
  include/bit/memory/adapters/standard_allocator.hpp

  # Policies
  # Growth Multipliers
  include/bit/memory/policies/growth_multipliers/linear_growth.hpp
  include/bit/memory/policies/growth_multipliers/no_growth.hpp
  include/bit/memory/policies/growth_multipliers/power_two_growth.hpp
  include/bit/memory/policies/growth_multipliers/quadratic_growth.hpp
  # Taggers
  include/bit/memory/policies/taggers/allocator_tagger.hpp
  include/bit/memory/policies/taggers/block_allocator_tagger.hpp
  include/bit/memory/policies/taggers/null_tagger.hpp
  # Trackers
  include/bit/memory/policies/trackers/detail/stat_recording_tracker.hpp
  include/bit/memory/policies/trackers/detailed_leak_tracker.hpp
  include/bit/memory/policies/trackers/leak_tracker.hpp
  include/bit/memory/policies/trackers/null_tracker.hpp
  include/bit/memory/policies/trackers/stdout_tracker.hpp
  # Bounds Checkers
  include/bit/memory/policies/bounds_checkers/debug_bounds_checker.hpp
  include/bit/memory/policies/bounds_checkers/null_bounds_checker.hpp
  # Lockables
  include/bit/memory/policies/lockables/null_lock.hpp

  # Block Allocators
  include/bit/memory/block_allocators/detail/cached_block_allocator.hpp
  include/bit/memory/block_allocators/detail/named_block_allocator.hpp
  include/bit/memory/block_allocators/aligned_block_allocator.hpp
  include/bit/memory/block_allocators/block_allocator_reference.hpp
  include/bit/memory/block_allocators/malloc_block_allocator.hpp
  include/bit/memory/block_allocators/new_block_allocator.hpp
  include/bit/memory/block_allocators/null_block_allocator.hpp
  include/bit/memory/block_allocators/policy_block_allocator.hpp
  include/bit/memory/block_allocators/stack_block_allocator.hpp
  include/bit/memory/block_allocators/static_block_allocator.hpp
  include/bit/memory/block_allocators/virtual_block_allocator.hpp

  # Block Allocator Storage
  include/bit/memory/block_allocator_storage/referenced_block_allocator_storage.hpp
  include/bit/memory/block_allocator_storage/shared_block_allocator_storage.hpp
  include/bit/memory/block_allocator_storage/stateless_block_allocator_storage.hpp

  # Allocators
  include/bit/memory/allocators/detail/named_allocator.hpp
  include/bit/memory/allocators/aligned_allocator.hpp
  include/bit/memory/allocators/aligned_offset_allocator.hpp
  include/bit/memory/allocators/allocator_reference.hpp
  include/bit/memory/allocators/bump_down_allocator.hpp
  include/bit/memory/allocators/bump_down_lifo_allocator.hpp
  include/bit/memory/allocators/bump_up_allocator.hpp
  include/bit/memory/allocators/bump_up_lifo_allocator.hpp
  include/bit/memory/allocators/fallback_allocator.hpp
  include/bit/memory/allocators/policy_allocator.hpp
  include/bit/memory/allocators/malloc_allocator.hpp
  include/bit/memory/allocators/new_allocator.hpp
  include/bit/memory/allocators/null_allocator.hpp
  include/bit/memory/allocators/pool_allocator.hpp
  include/bit/memory/allocators/stack_allocator.hpp

  # Allocator Storage
  include/bit/memory/allocator_storage/stateless_allocator_storage.hpp
  include/bit/memory/allocator_storage/shared_allocator_storage.hpp
  include/bit/memory/allocator_storage/referenced_allocator_storage.hpp
)

# Though the inline headers are not strictly used (or even required by CMake),
# this is added for IDEs like CLion to accurately track headers.
set(inline_headers
  # Utilities
  include/bit/memory/utilities/detail/address.inl
  include/bit/memory/utilities/detail/allocator_info.inl
  include/bit/memory/utilities/detail/debugging.inl
  include/bit/memory/utilities/detail/dynamic_size_type.inl
  include/bit/memory/utilities/detail/ebo_storage.inl
  include/bit/memory/utilities/detail/endian.inl
  include/bit/memory/utilities/detail/freelist.inl
  include/bit/memory/utilities/detail/memory_block.inl
  include/bit/memory/utilities/detail/memory_block_cache.inl
  include/bit/memory/utilities/detail/not_null.inl
  include/bit/memory/utilities/detail/pointer_utilities.inl
  include/bit/memory/utilities/detail/unaligned_storage.inl
  include/bit/memory/utilities/detail/uninitialized_storage.inl

  # Traits
  include/bit/memory/traits/detail/allocator_traits.inl
  include/bit/memory/traits/detail/block_allocator_traits.inl
  include/bit/memory/traits/detail/extended_allocator_traits.inl

  # Regions
  include/bit/memory/regions/detail/virtual_memory.inl

  # Adapters
  include/bit/memory/adapters/detail/allocator_deleter.inl
  include/bit/memory/adapters/detail/polymorphic_allocator_deleter.inl
  include/bit/memory/adapters/detail/standard_allocator.inl

  # Policies
  # Growth Multipliers
  include/bit/memory/policies/growth_multipliers/detail/linear_growth.inl
  include/bit/memory/policies/growth_multipliers/detail/no_growth.inl
  include/bit/memory/policies/growth_multipliers/detail/power_two_growth.inl
  include/bit/memory/policies/growth_multipliers/detail/quadratic_growth.inl
  # Taggers
  include/bit/memory/policies/taggers/detail/allocator_tagger.inl
  include/bit/memory/policies/taggers/detail/block_allocator_tagger.inl
  # Trackers
  include/bit/memory/policies/trackers/detail/detailed_leak_tracker.inl
  include/bit/memory/policies/trackers/detail/leak_tracker.inl
  include/bit/memory/policies/trackers/detail/stat_recording_tracker.inl
  include/bit/memory/policies/trackers/detail/stdout_tracker.inl
  # Bounds Checkers
  include/bit/memory/policies/bounds_checkers/detail/debug_bounds_checker.inl

  # Block Allocators
  include/bit/memory/block_allocators/detail/aligned_block_allocator.inl
  include/bit/memory/block_allocators/detail/block_allocator_reference.inl
  include/bit/memory/block_allocators/detail/cached_block_allocator.inl
  include/bit/memory/block_allocators/detail/malloc_block_allocator.inl
  include/bit/memory/block_allocators/detail/named_block_allocator.inl
  include/bit/memory/block_allocators/detail/new_block_allocator.inl
  include/bit/memory/block_allocators/detail/null_block_allocator.inl
  include/bit/memory/block_allocators/detail/policy_block_allocator.inl
  include/bit/memory/block_allocators/detail/stack_block_allocator.inl
  include/bit/memory/block_allocators/detail/static_block_allocator.inl
  include/bit/memory/block_allocators/detail/virtual_block_allocator.inl

  # Block Allocator Storage
  include/bit/memory/block_allocator_storage/detail/referenced_block_allocator_storage.inl
  include/bit/memory/block_allocator_storage/detail/shared_block_allocator_storage.inl
  include/bit/memory/block_allocator_storage/detail/stateless_block_allocator_storage.inl

  # Allocators
  include/bit/memory/allocators/detail/aligned_allocator.inl
  include/bit/memory/allocators/detail/aligned_offset_allocator.inl
  include/bit/memory/allocators/detail/allocator_reference.inl
  include/bit/memory/allocators/detail/bump_down_allocator.inl
  include/bit/memory/allocators/detail/bump_down_lifo_allocator.inl
  include/bit/memory/allocators/detail/bump_up_allocator.inl
  include/bit/memory/allocators/detail/bump_up_lifo_allocator.inl
  include/bit/memory/allocators/detail/fallback_allocator.inl
  include/bit/memory/allocators/detail/malloc_allocator.inl
  include/bit/memory/allocators/detail/named_allocator.inl
  include/bit/memory/allocators/detail/new_allocator.inl
  include/bit/memory/allocators/detail/null_allocator.inl
  include/bit/memory/allocators/detail/policy_allocator.inl
  include/bit/memory/allocators/detail/pool_allocator.inl
  include/bit/memory/allocators/detail/stack_allocator.inl

  # Allocator Storage
  include/bit/memory/allocator_storage/detail/stateless_allocator_storage.inl
  include/bit/memory/allocator_storage/detail/shared_allocator_storage.inl
  include/bit/memory/allocator_storage/detail/referenced_allocator_storage.inl
)

# Unfortunately, certain AppleClang versions don't support 'thread_local'; so to
# avoid failing independence tests, they have been appended here
if( NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  list(APPEND headers include/bit/memory/block_allocators/thread_local_block_allocator.hpp)
  list(APPEND inline_headers include/bit/memory/block_allocators/detail/thread_local_block_allocator.inl)
endif()


if( WIN32 )
  set(platform_source_files
    src/bit/memory/regions/win32/virtual_memory.cpp
    src/bit/memory/regions/win32/aligned_heap_memory.cpp
  )
elseif( UNIX )
  set(platform_source_files
    src/bit/memory/regions/posix/virtual_memory.cpp
    src/bit/memory/regions/posix/aligned_heap_memory.cpp
  )
elseif( APPLE )
  set(platform_source_files
    src/bit/memory/regions/posix/virtual_memory.cpp
    src/bit/memory/regions/posix/aligned_heap_memory.cpp
  )
else()
  message(FATAL_ERROR "unknown or unsupported target memory")
endif()

set(source_files
  # Utilities
  src/bit/memory/utilities/debugging.cpp
  src/bit/memory/utilities/errors.cpp

  # Regions
  src/bit/memory/regions/aligned_heap_memory.cpp
  src/bit/memory/regions/virtual_memory.cpp

  # memory-specific
  ${platform_source_files}
)

#-----------------------------------------------------------------------------
# bit::memory: Library
#-----------------------------------------------------------------------------

add_library(memory ${source_files} ${headers} ${inline_headers})
add_library(bit::memory ALIAS memory)
group_source_tree("${CMAKE_CURRENT_LIST_DIR}" ${source_files} ${headers} ${inline_headers})

# Add include directories
target_include_directories(memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

# Add DEBUG, NDEBUG, and RELEASE macro definitions
target_compile_definitions(memory PUBLIC
  $<$<CONFIG:DEBUG>:DEBUG>
  $<$<CONFIG:RELEASE>:RELEASE>
)

# Add compiler-specific flags
if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  target_compile_options(memory PRIVATE -Wall -Wstrict-aliasing -pedantic -Werror)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  target_compile_options(memory PRIVATE -Wall -Wstrict-aliasing -pedantic -Werror)
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # TODO: Determine MSVC necessary compiler flags
endif()

#-----------------------------------------------------------------------------
# bit::memory : Header-self-containment Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_HEADER_SELF_CONTAINMENT_TESTS )

  # Add containment test and alias as 'bit::memory::header_self_containment_test
  add_header_self_containment_test(memory_header_self_containment_test ${headers})
  add_library(bit::memory::header_self_containment_test ALIAS memory_header_self_containment_test)

  copy_target_properties(memory_header_self_containment_test memory PROPERTIES
    COMPILE_DEFINITIONS
    COMPILE_OPTIONS
    INCLUDE_DIRECTORIES
  )

endif()

#-----------------------------------------------------------------------------
# bit::memory : Unit Tests
#-----------------------------------------------------------------------------

if( BIT_MEMORY_COMPILE_UNIT_TESTS )
  add_subdirectory(test)
endif()

#-----------------------------------------------------------------------------
# bit::memory : Benchmarks
#-----------------------------------------------------------------------------

# if( BIT_MEMORY_COMPILE_BENCHMARKS )
#   add_subdirectory(benchmarks)
# endif()

#-----------------------------------------------------------------------------
# bit::memory : Documentation
#-----------------------------------------------------------------------------

if( BIT_MEMORY_GENERATE_DOCUMENTATION )
  add_subdirectory(doc)
endif()

#-----------------------------------------------------------------------------
# bit::memory : Export
#-----------------------------------------------------------------------------

include(ExportLibrary)

if( EXISTS "$ENV{BIT_HOME}" )
  set(CMAKE_INSTALL_PREFIX "$ENV{BIT_HOME}")
endif()

export_library( TARGETS memory
                PACKAGE Memory
                VERSION ${BIT_MEMORY_VERSION}
                MAJOR_VERSION ${BIT_MEMORY_VERSION_MAJOR}
                MINOR_VERSION ${BIT_MEMORY_VERSION_MINOR}
                INCLUDE_PATHS "${CMAKE_CURRENT_LIST_DIR}/include"
                              "${CMAKE_CURRENT_BINARY_DIR}/include"
                NAMESPACE "bit::"
                PREFIX_PACKAGE )

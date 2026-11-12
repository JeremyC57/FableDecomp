## 0x009F20B0

# Header overlay for row 0x009F20B0 (CMemoryAllocator::CMemoryAllocator)
Each file is a full copy of the current repo header plus `#ifdef` additions only (default path unchanged).
- engine/CMemoryAllocator.h: `FABLE_RETAIL_CMEMORYALLOCATOR_CTOR` -- Ego_r.pdb storage layout (0x49c), `CAllocatorInfo` / `CAllocSizeToAllocator` records, `extern g_AllocatorInfo[]` (0x01396868, descriptive name), protected ctor `??0CMemoryAllocator@@IAE@PAX@Z`, compile-time size check.
- engine/CMemoryAllocatorBase.h: `FABLE_RETAIL_CMEMORYALLOCATORBASE_OPERATOR_NEW` -- `static void* __fastcall operator new(unsigned int)` (`??2CMemoryAllocatorBase@@SIPAXI@Z`, 0x00A66730) inside the `FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE` class.
- engine/CMemoryAllocatorFixedSize.h: `FABLE_RETAIL_CMEMORYALLOCATORFIXEDSIZE_CLASS` -- real class deriving from the polymorphic CMemoryAllocatorBase (same field names/offsets as the flat projection), ctor `??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z` and virtual overrides; needs `FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE`.
- engine/CMemoryAllocatorVariableSize.h: `FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_CTOR` -- ctor `??0CMemoryAllocatorVariableSize@@QAE@PAXJ0JKKK@Z` and the virtual overrides (class becomes concrete); needs `FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE`.


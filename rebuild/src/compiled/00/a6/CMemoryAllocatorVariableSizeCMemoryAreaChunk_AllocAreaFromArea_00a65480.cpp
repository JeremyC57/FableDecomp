// 00a65480 CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea -- split a free area and link the allocated result.
// Leaf: ?AllocAreaFromArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@2@PAV32@J@Z
// Flags: none
// Retail:00a65480,265 bytes.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOC_FROM_AREA
#include "fable_compiler.h"
#include "engine/CMemoryAllocatorVariableSize.h"
CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* parea,long data_size)
{
 __int64 overflow_check=CMemoryArea::GetHeaderSize()+data_size;
 if(overflow_check<0||overflow_check>0x7fffffff)return 0;
 long total_area=CMemoryArea::GetHeaderSize()+data_size;
 if(parea->GetDataSize()>total_area){
  RemoveAreaFromFreeList(parea);
  long old_area_size=parea->GetTotalSize()-total_area-CMemoryArea::GetHeaderSize();
  CMemoryArea* pold_area=CMemoryArea::Create(parea,this,old_area_size);
  CMemoryArea* pnew_area=CMemoryArea::Create((char*)pold_area+parea->GetTotalSize(),this,data_size);
  InsertAreaIntoFreeList(pold_area);
  InsertAreaIntoAllocatedList(pnew_area);
  return pnew_area;
 }
 RemoveAreaFromFreeList(parea);
 InsertAreaIntoAllocatedList(parea);
 return parea;
}
// CMemoryAllocatorVariableSize::NewChunk -- acquire and link a variable-size allocation chunk.
// Leaf: ?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z
// Retail: 0x00a65720,269 bytes,thiscall.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#include "engine/CMemoryAllocatorVariableSize.h"
#include "engine/CMemoryAllocator.h"
struct _MEMORY_BASIC_INFORMATION {
 void* BaseAddress;void* AllocationBase;unsigned long AllocationProtect;
 unsigned long RegionSize,State,Protect,Type;
};
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void* FABLE_STDCALL VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL VirtualQuery(const void*,_MEMORY_BASIC_INFORMATION*,unsigned long);
#else
extern "C" void* VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long VirtualQuery(const void*,_MEMORY_BASIC_INFORMATION*,unsigned long);
#endif
inline long CMemoryAllocatorVariableSize::InternalAlloc(long no_bytes,void** pdata)
{
 unsigned long page_flags=0;
 if(Flags&2)page_flags|=2;else page_flags|=4;
 long size_allocated=0;
 *pdata=VirtualAlloc(0,no_bytes,0x103000,page_flags);
 if(*pdata){
  _MEMORY_BASIC_INFORMATION info;
  VirtualQuery(*pdata,&info,sizeof(info));
  size_allocated=info.RegionSize;
 }
 return size_allocated;
}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long request_size)
{
 long real_size=Align(CMemoryAreaChunk::GetHeaderSize()+request_size+CMemoryArea::GetHeaderSize()>(unsigned long)MinChunkSize?
  CMemoryAreaChunk::GetHeaderSize()+request_size+CMemoryArea::GetHeaderSize():MinChunkSize);
 void* pdata=0;
 long allocated_size=0;
 long try_size=real_size;
 while(pdata==0 && try_size>=request_size){
  allocated_size=InternalAlloc(try_size,&pdata);
  try_size/=2;
 }
 if(!pdata){CMemoryAllocator::Get()->OnFailedAllocation();return 0;}
 long data_size=allocated_size-CMemoryAreaChunk::GetHeaderSize();
 CMemoryAreaChunk* pchunk=(CMemoryAreaChunk*)pdata;
 pchunk->Init((char*)pdata+CMemoryAreaChunk::GetHeaderSize(),data_size,this);
 pchunk->PNextChunk=PFirstChunk;
 if(PFirstChunk)PFirstChunk->PPreviousChunk=pchunk;
 pchunk->PPreviousChunk=0;
 PFirstChunk=pchunk;
 return pchunk;
}
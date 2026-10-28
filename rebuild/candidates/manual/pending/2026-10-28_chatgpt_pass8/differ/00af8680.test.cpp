#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;if(entry || AllowHeapToGrow){if(!entry){CreateMoreBlocks();entry=FreeList;}++AllocCount;FreeList=entry->NextEntry;return entry;}return 0;}private:void CreateMoreBlocks();};
extern CDynamicObjectHeap<24,853> RandomAccelerationHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CRandomAcceleration :public CBaseClassNonCopyable{public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CRandomAcceleration::operator new(AllocationSize size){return RandomAccelerationHeap.Alloc((long)size);}}
CDynamicObjectHeap<24,853> RandomAccelerationHeap;static int growth,bad;CDynamicObjectHeapBase::~CDynamicObjectHeapBase(){}template<long Size,long Count>void CDynamicObjectHeap<Size,Count>::CreateMoreBlocks(){++growth;if(this!=&RandomAccelerationHeap || FreeList)bad=1;FreeList=&StartBlock.Blocks[0];FreeList->NextEntry=0;}
int main(){typedef CDynamicObjectHeap<24,853> Heap;Heap::CObjectEntry* first=&RandomAccelerationHeap.StartBlock.Blocks[0];Heap::CObjectEntry* second=&RandomAccelerationHeap.StartBlock.Blocks[1];first->NextEntry=second;second->NextEntry=0;RandomAccelerationHeap.FreeList=first;RandomAccelerationHeap.AllocCount=7;RandomAccelerationHeap.AllowHeapToGrow=false;if(NParticleEngine::CRandomAcceleration::operator new(24)!=first || RandomAccelerationHeap.FreeList!=second || RandomAccelerationHeap.AllocCount!=8 || growth)goto fail;if(NParticleEngine::CRandomAcceleration::operator new(24)!=second || RandomAccelerationHeap.FreeList || RandomAccelerationHeap.AllocCount!=9 || growth)goto fail;if(NParticleEngine::CRandomAcceleration::operator new(24) || RandomAccelerationHeap.FreeList || RandomAccelerationHeap.AllocCount!=9 || growth)goto fail;RandomAccelerationHeap.AllowHeapToGrow=true;if(NParticleEngine::CRandomAcceleration::operator new(24)!=first || RandomAccelerationHeap.FreeList || RandomAccelerationHeap.AllocCount!=10 || growth!=1 || bad)goto fail;puts("00af8680_TEST PASS");return 0;fail:puts("00af8680_TEST FAIL");return 1;}

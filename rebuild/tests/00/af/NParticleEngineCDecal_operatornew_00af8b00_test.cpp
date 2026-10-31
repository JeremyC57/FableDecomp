#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;private:virtual void Release()=0;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;if(!entry){if(AllowHeapToGrow){CreateMoreBlocks();entry=FreeList;}else return 0;}++AllocCount;FreeList=entry->NextEntry;return entry;}private:virtual void Release();void CreateMoreBlocks();};
extern CDynamicObjectHeap<116,529> DecalHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CDecal {public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CDecal::operator new(AllocationSize size){return DecalHeap.Alloc((long)size);}}
CDynamicObjectHeap<116,529> DecalHeap;static int growth,bad;CDynamicObjectHeapBase::~CDynamicObjectHeapBase(){}template<long Size,long Count>void CDynamicObjectHeap<Size,Count>::CreateMoreBlocks(){++growth;if(this!=&DecalHeap || FreeList)bad=1;FreeList=&StartBlock.Blocks[0];FreeList->NextEntry=0;}
template<long Size,long Count>void CDynamicObjectHeap<Size,Count>::Release(){}
int main(){typedef CDynamicObjectHeap<116,529> Heap;Heap::CObjectEntry* first=&DecalHeap.StartBlock.Blocks[0];Heap::CObjectEntry* second=&DecalHeap.StartBlock.Blocks[1];first->NextEntry=second;second->NextEntry=0;DecalHeap.FreeList=first;DecalHeap.AllocCount=7;DecalHeap.AllowHeapToGrow=false;if(NParticleEngine::CDecal::operator new(116)!=first || DecalHeap.FreeList!=second || DecalHeap.AllocCount!=8 || growth)goto fail;if(NParticleEngine::CDecal::operator new(116)!=second || DecalHeap.FreeList || DecalHeap.AllocCount!=9 || growth)goto fail;if(NParticleEngine::CDecal::operator new(116) || DecalHeap.FreeList || DecalHeap.AllocCount!=9 || growth)goto fail;DecalHeap.AllowHeapToGrow=true;if(NParticleEngine::CDecal::operator new(116)!=first || DecalHeap.FreeList || DecalHeap.AllocCount!=10 || growth!=1 || bad)goto fail;puts("00af8b00_TEST PASS");return 0;fail:puts("00af8b00_TEST FAIL");return 1;}
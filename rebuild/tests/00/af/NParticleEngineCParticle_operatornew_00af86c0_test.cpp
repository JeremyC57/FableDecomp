#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;private:virtual void Release()=0;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;if(!entry){if(AllowHeapToGrow){CreateMoreBlocks();entry=FreeList;}else return 0;}++AllocCount;FreeList=entry->NextEntry;return entry;}private:virtual void Release();void CreateMoreBlocks();};
extern CDynamicObjectHeap<156,2100> ParticleHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CParticle :public CBaseClassNonCopyable{public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CParticle::operator new(AllocationSize size){return ParticleHeap.Alloc((long)size);}}
CDynamicObjectHeap<156,2100> ParticleHeap;static int growth,bad;CDynamicObjectHeapBase::~CDynamicObjectHeapBase(){}template<long Size,long Count>void CDynamicObjectHeap<Size,Count>::CreateMoreBlocks(){++growth;if(this!=&ParticleHeap || FreeList)bad=1;FreeList=&StartBlock.Blocks[0];FreeList->NextEntry=0;}
template<long Size,long Count>void CDynamicObjectHeap<Size,Count>::Release(){}
int main(){typedef CDynamicObjectHeap<156,2100> Heap;Heap::CObjectEntry* first=&ParticleHeap.StartBlock.Blocks[0];Heap::CObjectEntry* second=&ParticleHeap.StartBlock.Blocks[1];first->NextEntry=second;second->NextEntry=0;ParticleHeap.FreeList=first;ParticleHeap.AllocCount=7;ParticleHeap.AllowHeapToGrow=false;if(NParticleEngine::CParticle::operator new(156)!=first || ParticleHeap.FreeList!=second || ParticleHeap.AllocCount!=8 || growth)goto fail;if(NParticleEngine::CParticle::operator new(156)!=second || ParticleHeap.FreeList || ParticleHeap.AllocCount!=9 || growth)goto fail;if(NParticleEngine::CParticle::operator new(156) || ParticleHeap.FreeList || ParticleHeap.AllocCount!=9 || growth)goto fail;ParticleHeap.AllowHeapToGrow=true;if(NParticleEngine::CParticle::operator new(156)!=first || ParticleHeap.FreeList || ParticleHeap.AllocCount!=10 || growth!=1 || bad)goto fail;puts("00af86c0_TEST PASS");return 0;fail:puts("00af86c0_TEST FAIL");return 1;}
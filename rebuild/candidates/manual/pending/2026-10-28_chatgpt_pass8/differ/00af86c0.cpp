// NParticleEngine::CParticle::operator new(unsigned int size) -- pop an object from the class heap, growing it when allowed.
// Leaf: ??2CParticle@NParticleEngine@@SIPAXI@Z
// Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<156,2100> instance ParticleHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at01427E1C is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF88F0 original private template method.
// Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;private:virtual void Release()=0;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;CObjectEntry* result;if(entry || AllowHeapToGrow){if(!entry){CreateMoreBlocks();entry=FreeList;}++AllocCount;FreeList=entry->NextEntry;result=entry;}else{result=0;}return result;}private:virtual void Release();void CreateMoreBlocks();};
extern CDynamicObjectHeap<156,2100> ParticleHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CParticle :public CBaseClassNonCopyable{public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CParticle::operator new(AllocationSize size){return ParticleHeap.Alloc((long)size);}}

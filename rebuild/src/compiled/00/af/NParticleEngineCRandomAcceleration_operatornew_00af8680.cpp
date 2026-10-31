// NParticleEngine::CRandomAcceleration::operator new(unsigned int size) -- pop an object from the class heap, growing it when allowed.
// Leaf: ??2CRandomAcceleration@NParticleEngine@@SIPAXI@Z
// Grade: RELOCATION_MATCH -- the class-heap pop written `entry = FreeList; if (!entry) { if (AllowHeapToGrow) {
//   CreateMoreBlocks(); entry = FreeList; } else return 0; }`: the return-0 block goes to the tail (`je`) and AllocCount
//   is an `inc mem`, as retail
// Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<24,853> instance RandomAccelerationHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at013D7E4C is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF88A0 original private template method.
// Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;if(!entry){if(AllowHeapToGrow){CreateMoreBlocks();entry=FreeList;}else return 0;}++AllocCount;FreeList=entry->NextEntry;return entry;}private:void CreateMoreBlocks();};
extern CDynamicObjectHeap<24,853> RandomAccelerationHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CRandomAcceleration :public CBaseClassNonCopyable{public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CRandomAcceleration::operator new(AllocationSize size){return RandomAccelerationHeap.Alloc((long)size);}}
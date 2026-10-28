// NParticleEngine::CDecal::operator new(unsigned int size) -- pop an object from the class heap, growing it when allowed.
// Leaf: ??2CDecal@NParticleEngine@@SIPAXI@Z
// Identity: proven; own allocator callers and actual sibling deleting destructor/RTTI, exact original PDB static fastcall operatornew API; real CDynamicObjectHeap<116,529> instance DecalHeap (existing descriptive global), FreeList+20/AllocCount+4; own AllowHeapToGrow at01436DF0 is member after embedded StartBlock, not a separate global; CreateMoreBlocks00AF8C00 original private template method.
// Layout: real dynamic-heap base20 and nested embedded blocks with actual ObjectSize/BlockCount, AllowHeapToGrow after block link. No shared heap header. Owner declarations supply static operator naming only; no owner object fields/sizeof accessed. Particle/Random actual CBaseClassNonCopyable hierarchy retained; Decal has no base.
#ifndef _MSC_VER
#define __fastcall
typedef __SIZE_TYPE__ AllocationSize;
#else
typedef unsigned int AllocationSize;
#endif
class CDynamicObjectHeapBase{public:virtual ~CDynamicObjectHeapBase();long AllocCount,TotalAllocatorBlocks;bool RefCountReleaseEnabled;CDynamicObjectHeapBase* HeapNext;private:virtual void Release()=0;};
template<long ObjectSize,long BlockCount>class CDynamicObjectHeap:public CDynamicObjectHeapBase{public:struct CObjectEntry{CObjectEntry* NextEntry;unsigned char Pad[ObjectSize-4];};struct CObjectAllocatorBlockEntry{CObjectEntry Blocks[BlockCount];CObjectAllocatorBlockEntry* NextEntry;};CObjectEntry* FreeList;CObjectAllocatorBlockEntry StartBlock;bool AllowHeapToGrow;void* Alloc(long){CObjectEntry* entry=FreeList;do{if(!entry){if(!AllowHeapToGrow)break;CreateMoreBlocks();entry=FreeList;}++AllocCount;FreeList=entry->NextEntry;return entry;}while(false);return 0;}private:virtual void Release();void CreateMoreBlocks();};
extern CDynamicObjectHeap<116,529> DecalHeap;
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};
namespace NParticleEngine{class CDecal {public:static void* __fastcall operator new(AllocationSize);};void* __fastcall CDecal::operator new(AllocationSize size){return DecalHeap.Alloc((long)size);}}

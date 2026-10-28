// StaticInitialization::Initialize_LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight() -- Initialize the fixed lighting object heap.
// Leaf: _$E1
// Identity: probable. Own RTTI CDynamicObjectHeap<56,16> and global01437450; own lighting allocation consumers corroborate element type/size. FableWin nameset contains dynamic initializer for LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight. CDynamicObjectHeapBase ctor00f36c60, base20, FreeList20, StartBlock24, sixteen entries stride56, NextEntry920, grow byte924. Base RefCountedRelease00f36ca0 dispatches slot1; base vtable slot1 is purecall. No engine header for these types.
// Layout: original heap template, typed base and embedded block. Nested record keys follow the landed same-template constructor; template APIs not used here omitted.
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();
    long AllocCount, TotalAllocatorBlocks;
    bool RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
    CDynamicObjectHeapBase();
protected:
    virtual void Release()=0;
};
template<long ObjectSize,long BlockCount> class CDynamicObjectHeap:public CDynamicObjectHeapBase {
public:
    struct CObjectEntry { CObjectEntry* NextEntry; unsigned char Pad[ObjectSize-4]; };
    struct CObjectAllocatorBlockEntry { CObjectEntry Blocks[BlockCount]; CObjectAllocatorBlockEntry* NextEntry; };
    CObjectEntry* FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool AllowHeapToGrow;
    __forceinline CDynamicObjectHeap(bool allowHeapToGrow) {
        AllowHeapToGrow=allowHeapToGrow;
        FreeList=0;
        StartBlock.NextEntry=0;
        for(long i=0;i<BlockCount;++i) {
            CObjectEntry* entry=&StartBlock.Blocks[i];
            entry->NextEntry=FreeList;
            FreeList=entry;
        }
    }
    virtual ~CDynamicObjectHeap();
protected:
    virtual void Release();
};
CDynamicObjectHeap<56,16> LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight(true);
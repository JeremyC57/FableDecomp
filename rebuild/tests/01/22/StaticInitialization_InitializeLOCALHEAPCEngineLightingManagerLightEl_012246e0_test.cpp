#include <stdio.h>
// StaticInitialization::Initialize_LOCAL_HEAP_CEngineLightingManager__LightElementLink() -- Initialize the fixed lighting object heap.
// Leaf: _$E1
// Identity: probable. Own RTTI CDynamicObjectHeap<12,16> and global01436ed0; own lighting allocation consumers corroborate element type/size. FableWin nameset contains dynamic initializer for LOCAL_HEAP_CEngineLightingManager__LightElementLink. CDynamicObjectHeapBase ctor00f36c60, base20, FreeList20, StartBlock24, sixteen entries stride12, NextEntry216, grow byte220. Base RefCountedRelease00f36ca0 dispatches slot1; base vtable slot1 is purecall. No engine header for these types.
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
CDynamicObjectHeap<12,16> LOCAL_HEAP_CEngineLightingManager__LightElementLink(true);

static int baseConstructions;
CDynamicObjectHeapBase::CDynamicObjectHeapBase():AllocCount(0),TotalAllocatorBlocks(0),RefCountReleaseEnabled(false),HeapNext(0){++baseConstructions;}
CDynamicObjectHeapBase::~CDynamicObjectHeapBase(){}
template<long N,long Count> CDynamicObjectHeap<N,Count>::~CDynamicObjectHeap(){}
template<long N,long Count> void CDynamicObjectHeap<N,Count>::Release(){}
int main(){
 typedef CDynamicObjectHeap<12,16> Heap; Heap& h=LOCAL_HEAP_CEngineLightingManager__LightElementLink;
 bool ok=baseConstructions==1&&h.AllowHeapToGrow&&h.AllocCount==0&&h.TotalAllocatorBlocks==0&&!h.RefCountReleaseEnabled&&h.StartBlock.NextEntry==0;
 Heap::CObjectEntry* entry=h.FreeList;
 for(int i=15;i>=0;--i){ok=ok&&entry==&h.StartBlock.Blocks[i];if(!entry){ok=false;break;}entry=entry->NextEntry;}
 ok=ok&&entry==0;
 Heap fixed(false);ok=ok&&!fixed.AllowHeapToGrow&&baseConstructions==2;
 entry=fixed.FreeList;
 for(int i=15;i>=0;--i){ok=ok&&entry==&fixed.StartBlock.Blocks[i];if(!entry){ok=false;break;}entry=entry->NextEntry;}
 ok=ok&&entry==0;
 puts(ok?"012246e0_TEST PASS":"012246e0_TEST FAIL");return ok?0:1;}
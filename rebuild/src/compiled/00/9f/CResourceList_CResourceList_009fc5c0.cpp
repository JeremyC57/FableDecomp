// CResourceList::~CResourceList() -- unhook the embedded list-head resource from its owning list, then unwind the CResource / CIVCountedPointeeBase / CFailedAllocationHandler vtables
// Leaf: ??1CResourceList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC5C0, 47 bytes (+ int3 padding).
// Callee: CResourceList::RemoveResource(CResource*) @ 0x009FC020 (landed; manifest "CResourceBank").
// Vtables: CResourceList 0x0129C808, CResource 0x012354A4, CIVCountedPointeeBase 0x012354D0,
// CFailedAllocationHandler 0x0129B874 (rtti-vtables.tsv).  Callers: 0x009B0458, 0x00A6A4AF,
// 0x00A6BE36, 0x00A6BFE5.
// Layout (Ego_r.pdb, engine/CResourceList.h + engine/CResource.h): CResourceList (60 bytes) = vptr
// +0, CResource Head +0x04 (vptr, IVRefCount, ResourceList +8, PrevResource +0xc, NextResource
// +0x10, ResourceSize, LastUsedFrame), ResourceCount +0x20 ... UnloadedThisFrame +0x38.  The engine
// headers are flattened PODs (`void* __vftable`), unusable as polymorphic bases, so the classes are
// modelled locally with the header's member names.
// Shape: user destructors at every level: CResourceList's vtable store, then the Head member's
// ~CResource (vtable store; `if (ResourceList) ResourceList->RemoveResource(this)`), its base
// ~CIVCountedPointeeBase (vtable store), and finally the base ~CFailedAllocationHandler (store).
class CResourceList;

class CFailedAllocationHandler {                // vtable 0x0129B874
public:
    virtual void v00();
    ~CFailedAllocationHandler() { }
};

class CIVCountedPointeeBase {                   // vtable 0x012354D0
public:
    virtual void v00();
    unsigned long IVRefCount;                   // +0x04
    ~CIVCountedPointeeBase() { }
};

class CResource : public CIVCountedPointeeBase {   // vtable 0x012354A4, 28 bytes
public:
    CResourceList* ResourceList;                // +0x08
    CResource* PrevResource;                    // +0x0c
    CResource* NextResource;                    // +0x10
    unsigned long ResourceSize;                 // +0x14
    unsigned long LastUsedFrame;                // +0x18
    inline ~CResource();
};

class CResourceList : public CFailedAllocationHandler {   // vtable 0x0129C808, 60 bytes
public:
    CResource Head;                             // +0x04
    unsigned long ResourceCount;                // +0x20
    long AllocatedMemory;                       // +0x24
    long MaximumMemory;                         // +0x28
    unsigned long CurrentFrame;                 // +0x2c
    unsigned long DebugStatsFrame;              // +0x30
    unsigned long UnloadDelay;                  // +0x34
    unsigned long UnloadedThisFrame;            // +0x38
    void RemoveResource(CResource* resource);   // 0x009FC020
    ~CResourceList();
};

inline CResource::~CResource()
{
    if (ResourceList)
        ResourceList->RemoveResource(this);
}

CResourceList::~CResourceList()
{
}
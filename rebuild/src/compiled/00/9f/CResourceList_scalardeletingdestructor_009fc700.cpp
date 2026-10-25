// CResourceList::`scalar deleting destructor'(unsigned int) -- destroy the list's embedded Head resource (it unlinks itself when still listed), unwind CIVCountedPointeeBase and CFailedAllocationHandler, flag-tested delete
// Leaf: ??_GCResourceList@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC700, 67 bytes (+13 int3 padding), thiscall, `ret 4`.
// Identity: slot 0 of the CResourceList vtable 0x0129C808 (rtti-vtables.tsv; the PDB name set lists
//   ??_GCResourceList@@UAEPAXI@Z); the member vptr stores are CResource 0x012354A4 then CIVCountedPointeeBase 0x012354D0,
//   the last store 0x0129B874 is the CFailedAllocationHandler base. The catalog label CWaterPatchMesh is propagated.
// Callees: CResourceList::RemoveResource(CResource&) 0x009FC020, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CResourceList (0x3c): vptr, Head (CResource, 0x1c) +0x04, ResourceCount +0x20 .. UnloadedThisFrame
//   +0x38; CResource: vptr, IVRefCount +0x04 (CIVCountedPointeeBase), ResourceList +0x08, Prev/Next +0x0c/+0x10,
//   ResourceSize +0x14, LastUsedFrame +0x18. No engine/ headers.
// Shape: the retail ~CResourceList body is empty (its vptr store survives because it is user-declared); the Head member's
//   inline ~CResource stores its vptr and runs `if (ResourceList) ResourceList->RemoveResource(*this)` (the member load is
//   scheduled above the store), ~CIVCountedPointeeBase stores its vptr, then the base's. The out-of-line ~CResourceList
//   defined in the TU emits the vtable, and VC7.1 inlines it into the deleting destructor.
class CResourceList;

class CIVCountedPointeeBase {                            // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() {}
    unsigned long IVRefCount;                            // +0x04
};

class CResource : public CIVCountedPointeeBase {         // vtable 0x012354A4
public:
    virtual ~CResource();
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};

class CFailedAllocationHandler {                         // vtable 0x0129B874
public:
    virtual ~CFailedAllocationHandler() {}
};

class CResourceList : public CFailedAllocationHandler {  // vtable 0x0129C808
public:
    virtual ~CResourceList();
    void RemoveResource(CResource& resource);            // 0x009FC020
    CResource     Head;                                  // +0x04
    unsigned long ResourceCount;                         // +0x20
    long          AllocatedMemory;                       // +0x24
    long          MaximumMemory;                         // +0x28
    unsigned long CurrentFrame;                          // +0x2c
    unsigned long DebugStatsFrame;                       // +0x30
    unsigned long UnloadDelay;                           // +0x34
    unsigned long UnloadedThisFrame;                     // +0x38
};

inline CResource::~CResource()
{
    if (ResourceList)
        ResourceList->RemoveResource(*this);
}

CResourceList::~CResourceList()
{
}
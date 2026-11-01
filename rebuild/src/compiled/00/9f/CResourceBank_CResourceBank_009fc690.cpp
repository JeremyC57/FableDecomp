// CResourceBank::~CResourceBank() -- destroy the loading queue and the resident-resource list (each CResourceList unhooks its embedded list-head CResource from its owning list), then the CBaseClassNonCopyable base
// Leaf: ??1CResourceBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC690, 103 bytes. Sole non-vtable callers include the ??_G 0x009FC750 (CResourceBank
//   vtable 0x0129C814 slot 0) and the owners' destructors (8 call sites + 1 tail jump).
// Callees: CResourceList::RemoveResource(CResource&) 0x009FC020 (twice, through the inlined ~CResource of each list's Head);
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Identity: the first store is the CResourceBank vtable 0x0129C814 (rtti-vtables.tsv), and its slot 0 0x009FC750 is the only
//   ??_G that calls this body; Ego_r.pdb names ??1CResourceBank@@UAE@XZ (a user-declared virtual dtor: the vtable store
//   stays, a compiler-generated one would drop it).
// Layout (Ego_r.pdb, struct_layouts_egor.tsv): CResourceBank 0x7C = CBaseClassNonCopyable (vptr) + CResourceList
//   ResourceList +0x04 + CResourceList LoadingQueue +0x40; CResourceList 0x3C = CFailedAllocationHandler (vptr) + CResource
//   Head +0x04 + counters; CResource 0x1C (engine/CResource.h retail opt-in) = CIVCountedPointeeBase + ResourceList +0x08 ...
// Shape: members destroyed in reverse order (LoadingQueue +0x40 first). Retail's resource TU defines ~CResource (0x009FC550)
//   and ~CResourceList (0x009FC5C0) out of line ABOVE this body; VC7.1 expands both here on its own (same-file plain
//   members), each ending in the CIVCountedPointeeBase / CFailedAllocationHandler vtable stores of their inline base dtors.
// Header debt: engine/CResourceList.h is flattened (vptr word + Head bytes), so CResourceList and its base
//   CFailedAllocationHandler (vtable 0x0129B874: slot 0 ??_G 0x009D5140, slot 1 pure HandleFailedAllocation) are local models
//   with the PDB member names.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
#include "engine/CBaseClassNonCopyable.h"

class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler() { }                              // slot 0
    virtual bool HandleFailedAllocation(unsigned long size) = 0;         // slot 1 (pure)
};

class CResourceList : public CFailedAllocationHandler {                  // vtable 0x0129C808, 0x3C bytes
public:
    CResourceList();                                                     // 0x009FC570
    virtual ~CResourceList();                                            // 0x009FC5C0 (??_G 0x009FC700)
    virtual bool HandleFailedAllocation(unsigned long size);             // slot 1  0x009FC540
    void RemoveResource(CResource& resource);                            // 0x009FC020
    CResource     Head;                                                  // +0x04
    unsigned long ResourceCount;                                         // +0x20
    long          AllocatedMemory;                                       // +0x24
    long          MaximumMemory;                                         // +0x28
    unsigned long CurrentFrame;                                          // +0x2C
    unsigned long DebugStatsFrame;                                       // +0x30
    unsigned long UnloadDelay;                                           // +0x34
    unsigned long UnloadedThisFrame;                                     // +0x38
};

class CResourceBank : public CBaseClassNonCopyable {                     // vtable 0x0129C814, 0x7C bytes
public:
    CResourceBank();                                                     // 0x009FC5F0
    virtual ~CResourceBank();
    CResourceList ResourceList;                                          // +0x04
    CResourceList LoadingQueue;                                          // +0x40
};

// The resource TU's out-of-line members above the bank destructor (retail order).
CResource::~CResource()
{
    if (ResourceList)
        ResourceList->RemoveResource(*this);
}

CResourceList::~CResourceList()
{
}

CResourceBank::~CResourceBank()
{
}
// CResourceList::ProcessLoadingQueue(bool limitTime, double endTime) -- walk the loading queue: abort the loads that may be aborted (when aborting is enabled), complete the ones that finished, restarting from the head after each removal; stop early once the time budget is spent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC250, length 260 bytes (`ret 0Ch`).
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double on the FPU); CResource virtual slots 4 IsResourceLoaded (+0x10),
// 5 OnResourceLoaded (+0x14), 6 CanAbortResourceLoad(long unusedFrames) const (+0x18, Ego_r.pdb ?CanAbortResourceLoad@CResource@@UBE_NJ@Z), 7 OnResourceLoadAborted (+0x1c)
// (vtable 0x012354A4: 0x0042D950/60/70/80, labels from the manifest). Global: the abort-loads enable byte 0x01396C5C
// (descriptive name).
// Layout: engine/CResourceList.h (Head CResource +0x04, ResourceCount +0x20, AllocatedMemory +0x24, CurrentFrame +0x2c);
// CResource as the landed copy ctor 0x0046649A (ResourceList +8, PrevResource +0xc, NextResource +0x10, ResourceSize +0x14,
// LastUsedFrame +0x18).
// Shape: the abort argument is `GetCurrentFrame() - resource->GetLastUsedFrame()` through two inline const accessors
// (the plain member subtraction loads CurrentFrame first and rotates every later scratch register); the removal is an inline member (`ResourceList = 0`, the inline CResource::ListRemove of 0x009FBFB0 -- its two zero
// stores share the hoisted ebx zero -- then the count/memory bookkeeping), expanded once per arm; the successor is read
// into ebp before the tests; `GFGetTime() > endTime` gives `fcomp qword ; test ah,41h ; je exit`.
#include "engine/CResourceList.h"

double __fastcall GFGetTime();                          // 0x009E1BC0
extern bool g_bAbortResourceLoads;                      // 0x01396C5C

struct CResource {                                      // vtable 0x012354A4, 0x1C bytes
    virtual ~CResource();
    virtual void v01();
    virtual void UnloadResource();                      // slot 2
    virtual unsigned long GetUnloadableMemory();        // slot 3
    virtual bool IsResourceLoaded();                    // slot 4 (+0x10)
    virtual void OnResourceLoaded();                    // slot 5 (+0x14)
    virtual bool CanAbortResourceLoad(long unusedFrames) const;   // slot 6 (+0x18)
    virtual void OnResourceLoadAborted();               // slot 7 (+0x1c)
    long           IVRefCount;                          // +0x04 (CIVCountedPointeeBase)
    CResourceList* ResourceList;                        // +0x08
    CResource*     PrevResource;                        // +0x0C
    CResource*     NextResource;                        // +0x10
    unsigned long  ResourceSize;                        // +0x14
    unsigned long  LastUsedFrame;                       // +0x18
    unsigned long GetLastUsedFrame() const { return LastUsedFrame; }
    void ListRemove()                                   // 0x009FBFB0 out of line elsewhere
    {
        if (NextResource) {
            if (PrevResource->NextResource == this && NextResource->PrevResource == this) {
                PrevResource->NextResource = NextResource;
                NextResource->PrevResource = PrevResource;
            }
            PrevResource = 0;
            NextResource = 0;
        }
    }
};

struct CResourceList_Queue : CResourceList {
    CResource* GetHead() { return reinterpret_cast<CResource*>(Head); }
    unsigned long GetCurrentFrame() const { return CurrentFrame; }
    void RemoveResource(CResource* resource)
    {
        resource->ResourceList = 0;
        resource->ListRemove();
        --ResourceCount;
        AllocatedMemory -= resource->ResourceSize;
    }
    void ProcessLoadingQueue(bool limitTime, double endTime);
};

void CResourceList_Queue::ProcessLoadingQueue(bool limitTime, double endTime)
{
    CResource* resource = GetHead()->NextResource;
    while (resource != GetHead()) {
        if (!resource)
            break;
        if (limitTime && GFGetTime() > endTime)
            break;
        CResource* next = resource->NextResource;
        if (g_bAbortResourceLoads && resource->CanAbortResourceLoad(GetCurrentFrame() - resource->GetLastUsedFrame())) {
            RemoveResource(resource);
            resource->OnResourceLoadAborted();
            resource = GetHead()->NextResource;
        }
        else if (resource->IsResourceLoaded()) {
            RemoveResource(resource);
            resource->OnResourceLoaded();
            resource = GetHead()->NextResource;
        }
        else
            resource = next;
    }
}
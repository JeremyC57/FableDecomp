// C3DAnimationInfo::GetAnimationStats() -- make sure the animation data is decoded (loading it synchronously if needed), touch it in its resource list and return its counted stats pointer
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A269C0, 144 bytes. Callees: C3DAnimationInfo::BeginLoading 0x00A26730 (bool),
// CResource::AddToResourceList 0x009FC440 (`if (ResourceList) ResourceList->...(this)`),
// CAnimationData::BuildSharedAnimation 0x00A24D70, CResource::ListRemove 0x009FBFB0, CResource::ListInsert 0x009FBF90.
// Model as the landed C3DAnimationInfo::TouchAnimation (0x00A26A50) / IsLoaded (0x00A23F50): Data +0xC;
// CAnimationData : CResource (ResourceList +8, PrevResource +0xC, NextResource +0x10, LastUsedFrame +0x18)
// holds the stats CCountedPointer {object +0x28, count block +0x2C} and the pending load task +0x30.
// Shape: the two inline `Data && member` tests re-test Data (the second test's null check is kept); the
// touch block is the inline resource-list move-to-tail; the result is the counted pointer's copy
// constructor into the hidden return slot (add-ref `inc [block]`), Data re-read after the calls.
// Name fix (agentNF6 2026-11-10): CResource::RemoveFromResourceList 0x009FC440 (?RemoveFromResourceList@CResource@@QAEXXZ, Ego_r.pdb / FableWin.pdb; body: if the resource is in a list (+8), the list unlinks it, call 0x009FC020).
class CResourceList;

class CResource { public:
    virtual void v00();
    unsigned long IVRefCount;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
    void RemoveFromResourceList();              // 0x009FC440
};

class CResourceList { public:
    long m_pad0;                           // +0
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};

#include "engine/CCountedPointer.h"


class C3DAnimationStats;
struct CAsyncLoadTask;

struct CAnimationData : CResource {
    unsigned char Animations[0xc];                     // +0x1c map (12 bytes in retail)
    CCountedPointer<const C3DAnimationStats> Stats;    // +0x28
    CAsyncLoadTask* LoadTask;                          // +0x30
    void BuildSharedAnimation();                       // 0x00A24D70
    void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};

class C3DAnimationInfo { public:                  // engine/C3DAnimationInfo.h offsets
    virtual void v00();
    void* Bank;                            // +4
    unsigned long Index;                   // +8
    CAnimationData* Data;                  // +0xc
    bool HasStats() const { return Data && Data->Stats.Data; }
    bool IsLoading() const { return Data && Data->LoadTask; }
    CCountedPointer<const C3DAnimationStats> GetAnimationStats();
public:
    void BeginLoading(bool) const;   // 0x00A26730
};

CCountedPointer<const C3DAnimationStats> C3DAnimationInfo::GetAnimationStats()
{
    if (!HasStats()) {
        if (!IsLoading())
            BeginLoading(true);
        Data->RemoveFromResourceList();
        Data->BuildSharedAnimation();
    }
    Data->Touch();
    return Data->Stats;
}
// CTCActionUseScriptedHook::`scalar deleting destructor'(unsigned int) -- its vtable, the global-TC removal, ~AnimationName / ~SoundName, ~PCameraTrack / ~PEntranceConnectedTo, ~CTCBase, free for flag bit 0
// Leaf: ??_GCTCActionUseScriptedHook
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe 0x0072A450, 75 bytes; CTCActionUseScriptedHook vtable 0x0126664C slot 0 (rtti-vtables.tsv).
// The manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (the destructor body), CCharString::~CCharString 0x0099EAE0
// (+0x44, +0x40), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (+0x38, +0x20), CTCBase::~CTCBase
// 0x00686830, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCActionUseScriptedHook (0x50 bytes, same offsets in retail; the 8-byte UIDs force
// 8-byte alignment of +0x18/+0x30). CTCBase restated as in the landed 0x007A62D0 (the header's vptr is a data word).
// Shape: a `__forceinline` user virtual destructor (a plain inline one stays out of line: 30v75) whose body calls RemoveFromGlobalTCs after the vtable store; members die in
// reverse order (the intelligent pointers' generated destructors call the base destructor directly).
#include "fable_compiler.h"
class CThing;

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCharString { public:
    void* m_pData;
    CCharString();
    ~CCharString();                                      // 0x0099EAE0
};

class CTCActionUseScriptedHook : public CTCBase { public:
    bool                Usable;                          // +0x0c
    bool                ForceConfirmation;               // +0x0d
    bool                WaitingForConfirmation;          // +0x0e
    bool                PerformAction;                   // +0x0f
    bool                TeleportToRegionEntrance;        // +0x10
    unsigned __int64    EntranceConnectedToUID;          // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;            // +0x20
    bool                EntranceLocked;                  // +0x28
    unsigned __int64    CameraTrackUID;                  // +0x30
    CIntelligentPointer<CThing> PCameraTrack;                    // +0x38
    CCharString         SoundName;                       // +0x40
    CCharString         AnimationName;                   // +0x44
    long                ReplacementObject;               // +0x48
    bool                ReversedOnMiniMap;               // +0x4c
    bool                HiddenOnMiniMap;                 // +0x4d
    FABLE_FORCEINLINE virtual ~CTCActionUseScriptedHook() { RemoveFromGlobalTCs(); }
};

CTCActionUseScriptedHook* NewCTCActionUseScriptedHook()
{
    return new CTCActionUseScriptedHook;
}
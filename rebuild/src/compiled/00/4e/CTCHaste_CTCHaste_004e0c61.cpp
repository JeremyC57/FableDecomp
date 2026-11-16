// CTCHaste::CTCHaste(CThing* thing) -- CTCBase(thing), own vtable, Multiplier = 1.0f, ToTerminate = false, PauseFrames = -1, then the empty Multipliers map
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0C61, 45 bytes. Manifest label "CScrollingViewport::CScrollingViewport"
// is propagated and rejected: the vtable stored is 0x012423CC = CTCHaste (rtti-vtables.tsv), and the landed
// factory 0x004E2B4B allocates 0x24 bytes for it (= the retail CTCHaste).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (landed; manifest "CEngineSubPrimitive2DClockSprite"),
// the Multipliers map constructor @ 0x004DF9A0 (out of line; manifest "CDisplayManager::CopyBackBufferToTexture"
// is a propagated name).
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`): the pass-through `push dword ptr [esp+8]`
// of the base argument and `or dword ptr [esi+14h],-1` for PauseFrames; `fld1 ; fstp` for the 1.0f
// Multiplier; the init-list scalars are scheduled around the vtable store and the map member's
// constructor is called last.
// Layout = Ego_r.pdb CTCHaste (PDB 40 bytes, retail 0x24: the map is 12 bytes): CTCBase 0xC {vptr,
// ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9}, Multiplier +0xC, ToTerminate +0x10,
// PauseFrames +0x14, Multipliers (map<void*, float>) +0x18. engine/CTCBase.h holds the layout only, so the
// base is modelled as in the landed CTCRockTrollShield constructor (0x007AEE10).
// Leaf: ??0CTCHaste@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCHaste@@QAE@AAVCThing@@@Z).

#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CHasteMultiplierMap {                     // std::map<void*, float> (retail 12 bytes)
    void* _Myhead;
    void* _Reserved;
    unsigned long _Mysize;
    CHasteMultiplierMap();                       // 0x004DF9A0
};

class CTCHaste : public CTCBase { public:                // vtable 0x012423CC
    float               Multiplier;              // +0x0C
    bool                ToTerminate;             // +0x10
    long                PauseFrames;             // +0x14
    CHasteMultiplierMap Multipliers;             // +0x18
    CTCHaste(CThing& thing);
};

CTCHaste::CTCHaste(CThing& thing)
    : CTCBase(thing)
    , Multiplier(1.0f)
    , ToTerminate(false)
    , PauseFrames(-1)
{
}
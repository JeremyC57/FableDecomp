// NCreatureMode::CModeDragonHover::~CModeDragonHover() -- implicit destructor: PTarget, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeDragonHover@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E31A0, 34 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is 0x008E3870 inside the scalar deleting destructor 0x008E3860 of
//   NCreatureMode::CModeDragonFireBeam (slot 0 of vtable 0x01285900; RTTI .?AVCModeDragonFireBeam@NCreatureMode@@ :
//   CModeDragonHover : CModeStandBase : CTCCreatureModeManager::CModeBase). That ??_G destroys FireBeam's own PBeamWeaponEffect
//   (+0x84) inline and then calls this body, so it is the base-class destructor; it stores no vtable, so it is the implicit
//   one. FableWin.pdb ??1CModeDragonHover@NCreatureMode@@UAE@XZ (0x024BF860, right after ??_G 0x024BF820; Ego_r inlines it). Members
//   (Ego_r.pdb, no STL container): Filter (CAnimFilterOfGroup) +0x68, PTarget (CIntelligentPointer<CThing>) +0x74,
//   UseHeadTurning +0x7C; sizeof 0x80; FireBeam adds Timer +0x80, PBeamWeaponEffect +0x84, LastExplodeFrame +0x8C, BeamActive
//   +0x90 (0x94). CModeDragonHover's own ??_G 0x00741C10 (vtable 0x0126845C) inlines its destructor, so this out-of-line copy is
//   the only ??1CModeDragonHover symbol in retail. The catalog label "CThingNoiseDef::~CThingNoiseDef" is propagated and wrong.
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10, ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@ 0x0122FFCC,
//   ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: the derived class's ??_G expands ~CModeDragonFireBeam and calls this base dtor out of line; retail's own CModeDragonFireBeam
//   constructor (??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@@Z, body not reconstructed here) emits that vtable / ??_G.
// Layout (NF247, q12682 header port): engine/NCreatureMode_CModeDragonHover.h (FABLE_RETAIL_JACKDRAGON_HOVER_PASS27 + the NF247 opt-in
//   FABLE_RETAIL_NCREATUREMODE_CMODEDRAGONHOVER_VIRTUALS: the (CThing&) ctor and slot 2-5 overrides), on CTCCreatureModeManager.h
//   (FABLE_RETAIL_CMODEBASE_VIRTUALS), CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + PASS22_VTABLE), CAnimFilterBase.h / CAnimFilterOfGroup.h
//   (INLINE_CTOR: the implicit, inline ~CAnimFilterOfGroup) and CIntelligentPointer.h. NCreatureMode::CModeDragonFireBeam has no header
//   anywhere under rebuild/include: it stays this row's local class (HEAD token) on the shared CModeDragonHover base.
// Vtable 0x01285900 (NF247, q12682): slots 3 / 5 / 6 now bind CModeDragonFireBeam's own overrides (retail 0x008E0780 / 0x008D6A50 / 0x008DBB00;
//   Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?FrameUpdate / ?OnActivate / ?OnDeactivate@CModeDragonFireBeam@NCreatureMode@@UAEXXZ), not the
//   CModeDragonHover / CModeBase names.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE                  // NF242 (q12677): CModeStandBase's slot 2 / 7 / 9 overrides (0x00862EC0 / 0x00861900 / 0x0073DA00)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING                            // GetAsCharArray / GetLength for the inline CAnimFilterOfGroup ctor
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_NCREATUREMODE_CMODEDRAGONHOVER_VIRTUALS
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode_CModeDragonHover.h"

namespace NCreatureMode {
class CModeDragonFireBeam : public CModeDragonHover {      // vtable 0x01285900
public:
    CModeDragonFireBeam(CThing& thing);                     // ??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@@Z
    CModeDragonFireBeam(CThing& thing, CThing& target, float duration);
    virtual unsigned long GetType() const;
    virtual void FrameUpdate();                             // slot 3, 0x008E0780
    virtual void OnActivate();                              // slot 5, 0x008D6A50
    virtual void OnDeactivate();                            // slot 6, 0x008DBB00
    long                        Timer;                      // +0x80
    CIntelligentPointer<CThing> PBeamWeaponEffect;          // +0x84
    long                        LastExplodeFrame;           // +0x8C
    bool                        BeamActive;                 // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CModeDragonFireBeam) == 0x94);

CModeDragonFireBeam::CModeDragonFireBeam(CThing& thing)
    : CModeDragonHover(thing)
{
}
}

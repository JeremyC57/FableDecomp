// CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator(CThing&) -- CTCBase(owner), own vtable, cleared state, empty emitter pointer
// Leaf: ??0CTCAICreatureWillPowerIndicator@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCAICreatureWillPowerIndicator@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0E30, length 58 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, then the
// CIntelligentPointer vtable 0x01238C6C. Own vtable 0x0126FF2C (rtti-vtables.tsv); allocated with 0x24
// bytes by the landed factory 0x004D5E14.
//
// Layout = Ego_r.pdb CTCAICreatureWillPowerIndicator (0x24 bytes, byte-packed): CTCBase 0xc, PoweredUp
// +0x0c, Timer +0x10, ResetCount +0x14, ShowEffect +0x18 and the CIntelligentPointer<CThing> PEmitter at
// the UNALIGNED +0x19 -- the class is compiled under pack(1) (`lea edi,[esi+19h]`).
// Shape: base constructor, own vtable, the four init-list scalars from one zero register (ShowEffect =
// true), then the member pointer's base constructor and its derived vtable through edi. engine/CTCBase.h
// holds the layout only, so the base is modelled as `CTCBase_Calls` (as the landed CTCRockTrollShield).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

#pragma pack(push, 1)

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCAICreatureWillPowerIndicator : public CTCBase { public:    // vtable 0x0126FF2C
    bool                PoweredUp;                          // +0x0c
    unsigned char       _pad_0x0d[3];
    long                Timer;                              // +0x10
    long                ResetCount;                         // +0x14
    bool                ShowEffect;                         // +0x18
    CIntelligentPointer<CThing> PEmitter;                           // +0x19
    CTCAICreatureWillPowerIndicator(CThing& owner);
};
#pragma pack(pop)

CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator(CThing& owner)
    : CTCBase(owner)
    , PoweredUp(false)
    , Timer(0)
    , ResetCount(0)
    , ShowEffect(true)
    , PEmitter()
{
}
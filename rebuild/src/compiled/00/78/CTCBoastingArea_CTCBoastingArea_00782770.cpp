// CTCBoastingArea::CTCBoastingArea(CThing* thing) -- CTCBase(thing), own vtable, HeroWithinRadiusState = 0, Radius = 3.0, AnticipationRadius = 10.0
// Leaf: ??0CTCBoastingArea@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCBoastingArea@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782770, length 46 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated).
// Identity: vtable 0x0126B314 = CTCBoastingArea (rtti-vtables.tsv; its slot 14 is 0x00782A90, the
// OnKill of this chunk); the manifest label "CTCBed::CTCBed" is propagated.
// Layout = Ego_r.pdb CTCBoastingArea (24 bytes): HeroWithinRadiusState +0xc, Radius +0x10,
// AnticipationRadius +0x14. Shape (as the landed 0x007AEE10): constant initialisers are immediate
// stores after the vtable store (0x40400000 = 3.0f, 0x41200000 = 10.0f).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

enum EHeroWithinRadiusState { HERO_OUTSIDE_RADIUS = 0 };

struct CTCBoastingArea : CTCBase {                    // vtable 0x0126B314
    EHeroWithinRadiusState HeroWithinRadiusState;           // +0x0c
    float                  Radius;                          // +0x10
    float                  AnticipationRadius;              // +0x14
    CTCBoastingArea(CThing& thing);
};

CTCBoastingArea::CTCBoastingArea(CThing& thing)
    : CTCBase(thing)
    , HeroWithinRadiusState(HERO_OUTSIDE_RADIUS)
    , Radius(3.0f)
    , AnticipationRadius(10.0f)
{
}
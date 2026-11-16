// CTCPerceiveThing::GetSightRadius() const -- SightRadius scaled by the def's day-time / night-time visibility multiplier (their average at dusk and dawn)
// Leaf: ?GetSightRadius@CTCPerceiveThing@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007448F0, 81 bytes (catalogued as GetExtendedSightRadius; it scales SightRadius +0x40 -- the
//   0x00744950 twin scales ExtendedSightRadius +0x44).
// Callees: CTCBase::PeekWorld 0x00686A80, CGameTimeManager::IsDay 0x006BB2C0 and ::IsNightTime 0x006BB210 on the world's +0x1C. 0x0122F59C = 0.5f.
// Shape: the inline visibility multiplier (out-of-line copy 0x00744580, GetTimeBasedVisibilityMultiplier) times the radius, multiplier loaded
//   first; each arm returns on its own (`fmul [esi+0x40] ; pop esi ; ret` three times).
// NF39 (2026-11-16): moved off its `CTCPerceiveThing_Methods` stand-in (a renaming-macro layout view with local CTCBase / CWorld /
//   CGameTimeManager models) onto the shared engine/CTCPerceiveThing.h (SMELL branch: the Ego_r.pdb class, SightRadius +0x40, PDef +0x34;
//   new nested opt-in FABLE_RETAIL_CTCPERCEIVETHING_TIMEVISIBILITY_INLINE: the GetTimeBasedVisibilityMultiplier body), CWorld.h,
//   CGameTimeManager.h and CPerceivedThingDef.h. Evidence: Ego_r.pdb order GetSightRadius 0x007573B0 then GetExtendedSightRadius 0x00757430 =
//   retail 0x007448F0 / 0x00744950 (+0x40 / +0x44 = SightRadius / ExtendedSightRadius); the 11 landed callers already bind this spelling.
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_TIMEVISIBILITY_INLINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#include "engine/CTCPerceiveThing.h"

float CTCPerceiveThing::GetSightRadius() const
{
    return GetTimeBasedVisibilityMultiplier() * SightRadius;
}

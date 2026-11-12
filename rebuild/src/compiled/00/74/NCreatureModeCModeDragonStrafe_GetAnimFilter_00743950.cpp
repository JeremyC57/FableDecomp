// NCreatureMode::CModeDragonStrafe::GetAnimFilter() const -- returns the mode's own animation filter (Filter)
// Leaf: ?GetAnimFilter@CModeDragonStrafe@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743950, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeDragonStrafe's vtable 0x012686C8 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeDragonStrafe); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeDragonStrafe@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeDragonStrafe::vfunc_4 (rebuild/src/compiled/00/74/gapscan_sub00743950_00743950.cpp).
// No shared header for the NCreatureMode dragon modes: minimal local definitions (RTTI: CModeDragonFlying / CModeDragonStrafe :
// CModeDragonFlyingBase : CTCCreatureModeManager::CModeBase; CModeDragonStrafeBurninate : CModeDragonFlying). Ego_r.pdb puts
// CModeDragonFlyingBase at 0x3c (FlightSpline CCardinalSpline +0x1c, SplinePercentageComplete +0x34, LegAnimAlpha +0x38) and the
// derived Filter (CAnimFilterOfGroup) at +0x3c; retail reads Filter at +0x38 (`lea eax,[ecx+38h]`), 4 lower: the spline holds a
// retail STLport vector (0xc, Ego_r 0x10), as other retail layouts (opaque here).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeDragonFlyingBase : public CTCCreatureModeManager::CModeBase {
public:
    CModeDragonFlyingBase(CThing& thing);
protected:
    unsigned char FlightSpline[0x14];                                    // +0x1c CCardinalSpline (0x18 in Ego_r.pdb)
    float SplinePercentageComplete;                                      // +0x30
    float LegAnimAlpha;                                                  // +0x34
};
class CModeDragonStrafe : public CModeDragonFlyingBase {
public:
    CModeDragonStrafe(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
protected:
    CAnimFilterOfGroup Filter;                                           // +0x38
};
const CAnimFilterBase& CModeDragonStrafe::GetAnimFilter() const
{
    return Filter;
}
}

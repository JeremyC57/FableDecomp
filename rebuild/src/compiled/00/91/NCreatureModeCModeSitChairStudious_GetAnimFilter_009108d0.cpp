// NCreatureMode::CModeSitChairStudious::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOf3Groups)
// Leaf: ?GetAnimFilter@CModeSitChairStudious@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009108D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeSitChairStudious's vtable 0x0128F590 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeSitChairStudious); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeSitChairStudious@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeSitChairStudious::vfunc_4 (rebuild/src/compiled/00/91/gapscan_sub009108d0_009108d0.cpp).
// No shared header for NCreatureMode::CModeSitChairStudious: minimal local definition on CModeSitBase (engine/NCreatureMode_CModeSitFloor.h;
// RTTI base NCreatureMode::CModeSitBase; Ego_r.pdb Filter CAnimFilterOf3Groups +0x2c, size 0x48; retail `lea eax,[ecx+2Ch]`). No
// header for CAnimFilterOf3Groups (0x1c bytes): opaque.
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
#include "engine/NCreatureMode_CModeSitFloor.h"
namespace NCreatureMode {
class CModeSitChairStudious : public CModeSitBase {
public:
    CModeSitChairStudious(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char Filter[0x1c];                                          // +0x2c CAnimFilterOf3Groups
};
const CAnimFilterBase& CModeSitChairStudious::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
}

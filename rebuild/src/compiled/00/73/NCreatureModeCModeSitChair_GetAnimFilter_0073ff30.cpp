// NCreatureMode::CModeSitChair::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOf2Groups)
// Leaf: ?GetAnimFilter@CModeSitChair@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FF30, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeSitChair's vtable 0x01267B74 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeSitChair); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeSitChair@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeSitChair::vfunc_4 (rebuild/src/compiled/00/73/gapscan_sub0073ff30_0073ff30.cpp).
// No shared header for NCreatureMode::CModeSitChair: minimal local definition on CModeSitBase (engine/NCreatureMode_CModeSitFloor.h;
// RTTI base NCreatureMode::CModeSitBase; Ego_r.pdb Filter CAnimFilterOf2Groups +0x2c, size 0x40; retail `lea eax,[ecx+2Ch]`). No
// header for CAnimFilterOf2Groups (0x14 bytes): opaque.
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
class CModeSitChair : public CModeSitBase {
public:
    CModeSitChair(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char Filter[0x14];                                          // +0x2c CAnimFilterOf2Groups
};
const CAnimFilterBase& CModeSitChair::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
}

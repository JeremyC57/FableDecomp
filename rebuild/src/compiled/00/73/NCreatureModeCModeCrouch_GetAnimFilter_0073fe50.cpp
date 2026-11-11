// NCreatureMode::CModeCrouch::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOf2Groups)
// Leaf: ?GetAnimFilter@CModeCrouch@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073FE50, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeCrouch's vtable 0x01267B34 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeCrouch); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeCrouch@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeCrouch::vfunc_4 (rebuild/src/compiled/00/73/gapscan_sub0073fe50_0073fe50.cpp).
// No shared header for NCreatureMode::CModeCrouch: minimal local definition (RTTI base CTCCreatureModeManager::CModeBase; Ego_r.pdb
// SneakAnimName CCharString +0x1c, Filter CAnimFilterOf2Groups +0x20, size 0x34; retail `lea eax,[ecx+20h]`). No header for
// CAnimFilterOf2Groups (0x14 bytes): opaque.
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
namespace NCreatureMode {
class CModeCrouch : public CTCCreatureModeManager::CModeBase {
public:
    CModeCrouch(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    CCharString SneakAnimName;                                           // +0x1c
    unsigned char Filter[0x14];                                          // +0x20 CAnimFilterOf2Groups
};
const CAnimFilterBase& CModeCrouch::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
}

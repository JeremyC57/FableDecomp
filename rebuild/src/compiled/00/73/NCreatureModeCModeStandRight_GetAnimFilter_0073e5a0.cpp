// NCreatureMode::CModeStandRight::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOf3Groups)
// Leaf: ?GetAnimFilter@CModeStandRight@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E5A0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeStandRight's vtable 0x01267534 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeStandRight); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeStandRight@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeStandRight::vfunc_4 (rebuild/src/compiled/00/73/gapscan_sub0073e5a0_0073e5a0.cpp).
// No shared header for NCreatureMode::CModeStandRight: minimal local definition (RTTI base CModeStandBase; Ego_r.pdb Filter
// CAnimFilterOf3Groups at +0x68, size 0x84, which retail agrees with: `lea eax,[ecx+68h]`). No header for CAnimFilterOf3Groups (0x1c
// bytes): opaque.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
namespace NCreatureMode {
class CModeStandRight : public CModeStandBase {
public:
    CModeStandRight(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char Filter[0x1c];                                          // +0x68 CAnimFilterOf3Groups
};
const CAnimFilterBase& CModeStandRight::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
}

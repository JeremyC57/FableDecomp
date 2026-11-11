// NCreatureMode::CModeStandFront::GetAnimFilter() const -- returns the mode's own animation filter (Filter, a CAnimFilterOf3Groups)
// Leaf: ?GetAnimFilter@CModeStandFront@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E3C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeStandFront's vtable 0x012674AC (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeStandFront); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeStandFront@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeStandFront::vfunc_4 (rebuild/src/compiled/00/73/gapscan_sub0073e3c0_0073e3c0.cpp).
// No shared header for NCreatureMode::CModeStandFront: minimal local definition (RTTI base CModeStandBase; Ego_r.pdb Filter
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
class CModeStandFront : public CModeStandBase {
public:
    CModeStandFront(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char Filter[0x1c];                                          // +0x68 CAnimFilterOf3Groups
};
const CAnimFilterBase& CModeStandFront::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(Filter);
}
}

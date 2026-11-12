// CTCCameraPointBuilding::OnSerialseLeaf(CPersistContext&) -- persists ExitPoint (+0x41) and StartPoint (+0x42) through CPersistContext::Transfer<bool>, default false
// Leaf: ?OnSerialseLeaf@CTCCameraPointBuilding@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075D0A0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 33 of CTCCameraPointBuilding's vtable 0x0123ECBC (the slot holds this body in 1 vtable(s), owner CTCCameraPointBuilding); the Xbox 360 Test PDB virtual order (third source) aligned on 18 TLC-named slots of that vtable puts OnSerialseLeaf(CPersistContext &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialseLeaf@CTCCameraPointBuilding@@UAEXAAVCPersistContext@@@Z; `ret` matches the signature.
// Was: CTCCameraPointBuilding::vfunc_33 (rebuild/src/compiled/00/75/CTCCameraPointBuilding_vfunc33_0075d0a0.cpp).
// No shared header for CTCCameraPointBuilding: minimal local definition on its RTTI base CTCCameraPointDefinitionBase
// (shared header CTCCameraPointGeneralCase.h), Ego_r.pdb members at their offsets.
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCCameraPointGeneralCase.h"
class CTCCameraPointBuilding : public CTCCameraPointDefinitionBase {
public:
    virtual void OnSerialseLeaf(CPersistContext&);                          // slot 33
private:
    bool InMyBuilding;                                                      // +0x40
    bool ExitPoint;                                                         // +0x41
    bool StartPoint;                                                        // +0x42
};
void CTCCameraPointBuilding::OnSerialseLeaf(CPersistContext& context)
{
    context.Transfer("ExitPoint", ExitPoint, false);
    context.Transfer("StartPoint", StartPoint, false);
}

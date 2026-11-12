// CTCCameraPointFixedPoint::OnSerialseLeaf(CPersistContext&) -- persists TrackThing (+0x40, default true) and the three LookVector components (+0x41, default 0.0f) through CPersistContext::Transfer
// Leaf: ?OnSerialseLeaf@CTCCameraPointFixedPoint@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075D140, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 33 of CTCCameraPointFixedPoint's vtable 0x0123AF8C (the slot holds this body in 1 vtable(s), owner CTCCameraPointFixedPoint); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts OnSerialseLeaf(CPersistContext &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialseLeaf@CTCCameraPointFixedPoint@@UAEXAAVCPersistContext@@@Z; `ret` matches the signature.
// Was: CTCCameraPointFixedPoint::vfunc_33 (rebuild/src/compiled/00/75/CTCCameraPointFixedPoint_vfunc33_0075d140.cpp).
// No shared header for CTCCameraPointFixedPoint: minimal local definition on its RTTI base CTCCameraPointShapeActivationBase
// (shared header CTCCameraPointGeneralCase.h), Ego_r.pdb members at their offsets (LookVector at +0x41 unaligned, as in Ego_r.pdb
// and in retail: `lea eax,[esi+41h]`, `[esi+45h]`, `[esi+49h]`, so the members are byte-packed).
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS
#define FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VSLOTS3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCCameraPointGeneralCase.h"
#pragma pack(push, 1)
class CTCCameraPointFixedPoint : public CTCCameraPointShapeActivationBase {
public:
    virtual void OnSerialseLeaf(CPersistContext&);                          // slot 33
private:
    bool TrackThing;                                                        // +0x40
    C3DVector LookVector;                                                   // +0x41
};
#pragma pack(pop)
void CTCCameraPointFixedPoint::OnSerialseLeaf(CPersistContext& context)
{
    context.Transfer("TrackThing", TrackThing, true);
    context.Transfer("LookVector.X", LookVector.X, 0.0f);
    context.Transfer("LookVector.Y", LookVector.Y, 0.0f);
    context.Transfer("LookVector.Z", LookVector.Z, 0.0f);
}

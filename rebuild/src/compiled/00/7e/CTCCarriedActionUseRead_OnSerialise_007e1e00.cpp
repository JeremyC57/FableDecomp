// CTCCarriedActionUseRead::OnSerialise(CPersistContext&, bool) -- persists AlreadyRead (+0x18) through CPersistContext::Transfer<bool>, default false
// Leaf: ?OnSerialise@CTCCarriedActionUseRead@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E1E00, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCCarriedActionUseRead's vtable 0x01240834 (the slot holds this body in 1 vtable(s), owner CTCCarriedActionUseRead); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCCarriedActionUseRead@@MAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCCarriedActionUseRead::vfunc_1 (rebuild/src/compiled/00/7e/CTCCarriedActionUseRead_vfunc1_007e1e00.cpp).
// No shared header for CTCCarriedActionUseRead or its RTTI base CTCCarriedActionUseBase (on CTCActionUseBase, Ego_r.pdb
// sizeof 0x14, no members of its own): minimal local definitions with the Ego_r.pdb members at their offsets.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCActionUseBase.h"
class CCarriedReadableDef;
class CTCCarriedActionUseBase : public CTCActionUseBase {
};
class CTCCarriedActionUseRead : public CTCCarriedActionUseBase {
protected:
    virtual void OnSerialise(CPersistContext&, bool);                       // slot 1
private:
    CDefPointer<const CCarriedReadableDef> PDef;                            // +0x14
    bool AlreadyRead;                                                       // +0x18
};
void CTCCarriedActionUseRead::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("AlreadyRead", AlreadyRead, false);
}

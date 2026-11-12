// CTCRandomAppearanceMorph::OnSerialise(CPersistContext&, bool) -- persists Seed (+0x18) through CPersistContext::Transfer<unsigned long>, default 0
// Leaf: ?OnSerialise@CTCRandomAppearanceMorph@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727BC0, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCRandomAppearanceMorph's vtable 0x0126653C (the slot holds this body in 1 vtable(s), owner CTCRandomAppearanceMorph); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCRandomAppearanceMorph@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCRandomAppearanceMorph::vfunc_1 (rebuild/src/compiled/00/72/CTCRandomAppearanceMorph_vfunc1_00727bc0.cpp).
// No shared header for CTCRandomAppearanceMorph or its RTTI base CTCAppearanceMorphBase (CTCBase at +0, Ego_r.pdb sizeof 0xc):
// minimal local definitions with the Ego_r.pdb members at their offsets (NGraphicAppearance::CAGraphicAppearanceComponent is
// the second base, at +0xc per the RTTI base list).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
class CTCAppearanceMorphBase : public CTCBase {
public:
    class CMeshMorphSet;
};
class CTCRandomAppearanceMorph : public CTCAppearanceMorphBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
    virtual void OnSerialise(CPersistContext&, bool);                       // slot 1
private:
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;   // +0x10
    unsigned long Seed;                                                     // +0x18
};
void CTCRandomAppearanceMorph::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("Seed", Seed, 0ul);
}

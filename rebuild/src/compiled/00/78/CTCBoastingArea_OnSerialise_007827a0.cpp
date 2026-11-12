// CTCBoastingArea::OnSerialise(CPersistContext&, bool) -- persists Radius (+0x10) through CPersistContext::Transfer<float>, default 0.0f
// Leaf: ?OnSerialise@CTCBoastingArea@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007827A0, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCBoastingArea's vtable 0x0126B314 (the slot holds this body in 1 vtable(s), owner CTCBoastingArea); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCBoastingArea@@MAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCBoastingArea::vfunc_1 (rebuild/src/compiled/00/78/CTCBoastingArea_vfunc1_007827a0.cpp).
// No shared header for CTCBoastingArea: minimal local definition on its RTTI base CTCBase, Ego_r.pdb members at their
// offsets (the scope of the EHeroWithinRadiusState enum is not recorded in either PDB name set; it is declared nested here and
// appears in no symbol of this row).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCBase.h"
class CTCBoastingArea : public CTCBase {
public:
    enum EHeroWithinRadiusState { };
protected:
    virtual void OnSerialise(CPersistContext&, bool);           // slot 1
private:
    EHeroWithinRadiusState HeroWithinRadiusState;               // +0x0c
    float Radius;                                               // +0x10
    float AnticipationRadius;                                   // +0x14
};
void CTCBoastingArea::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("Radius", Radius, 0.0f);
}

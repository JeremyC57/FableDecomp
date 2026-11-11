// CTCWallMountEffects::OnSerialise(CPersistContext&, bool) -- persists FrameEffectLastUsed (+0x1c) through CPersistContext::Transfer<long>, default 0
// Leaf: ?OnSerialise@CTCWallMountEffects@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DF670, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCWallMountEffects's vtable 0x0126FC84 (the slot holds this body in 1 vtable(s), owner CTCWallMountEffects); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCWallMountEffects@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCWallMountEffects::vfunc_1 (rebuild/src/compiled/00/7d/CTCWallMountEffects_vfunc1_007df670.cpp).
// No shared header for CTCWallMountEffects: minimal local definition on its RTTI base CTCBase, Ego_r.pdb members at their
// offsets (the scope of the EWallMountEffectsConfirmationType enum is not recorded in either PDB name set; declared nested
// here, it appears in no symbol of this row).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CWallMountEffectsDef;
class CThingCreatureBase;
class CTCWallMountEffects : public CTCBase {
public:
    enum EWallMountEffectsConfirmationType { };
    virtual void OnSerialise(CPersistContext&, bool);                       // slot 1
private:
    CDefPointer<const CWallMountEffectsDef> PDef;                           // +0x0c
    EWallMountEffectsConfirmationType WaitingForConfirmation;               // +0x10
    CIntelligentPointer<CThingCreatureBase> CreaturePerformingAction;       // +0x14
    long FrameEffectLastUsed;                                               // +0x1c
    bool ObjectWasGamePersistant;                                           // +0x20
};
void CTCWallMountEffects::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("FrameEffectLastUsed", FrameEffectLastUsed, 0L);
}

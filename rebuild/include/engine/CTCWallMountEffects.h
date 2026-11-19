#ifndef FABLE_ENGINE_CTCWALLMOUNTEFFECTS_H
#define FABLE_ENGINE_CTCWALLMOUNTEFFECTS_H
// CTCWallMountEffects (new header, hand3c, row 0x007DF770 FrameUpdate): RTTI .?AVCTCWallMountEffects@@, single base CTCBase at +0
// (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv, 36 bytes, no STL container): PDef
// (CDefPointer<const CWallMountEffectsDef>) +0x0C, WaitingForConfirmation (EWallMountEffectsConfirmationType) +0x10,
// CreaturePerformingAction (CIntelligentPointer<CThingCreatureBase>) +0x14, FrameEffectLastUsed +0x1C, ObjectWasGamePersistant +0x20.
// EWallMountEffectsConfirmationType: FableWin.pdb LF_FIELDLIST 0x3408E. Only the members and methods rows use are modelled.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CThingCreatureBase; class CWallMountEffectsDef;
enum EWallMountEffectsConfirmationType {
    CONFIRMATION_NONE = 0,
    CONFIRMATION_EFFECT = 1,
    CONFIRMATION_REMOVE = 2
};
class CTCWallMountEffects : public CTCBase {
public:
    virtual void FrameUpdate();                                          // slot 9, 0x007DF770 ?FrameUpdate@CTCWallMountEffects@@UAEXXZ
private:
    void DoEffect();                                                     // 0x007DF6A0 ?DoEffect@CTCWallMountEffects@@AAEXXZ
    CDefPointer<const CWallMountEffectsDef> PDef;                        // +0x0c
    EWallMountEffectsConfirmationType WaitingForConfirmation;            // +0x10
    CIntelligentPointer<CThingCreatureBase> CreaturePerformingAction;    // +0x14
    long FrameEffectLastUsed;                                            // +0x1c
    bool ObjectWasGamePersistant;                                        // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CTCWallMountEffects) == 0x24);
#endif

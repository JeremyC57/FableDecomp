// CCombatActionBase::ResetMeleeCombatAbilityEffects() -- unless already reset, fetch the melee ability's knockdown-effects def; when it paused the entities, drop this action's haste multiplier on the attacker and on a live target and remove its bullet-time slowdown; when it set an environment theme, transition every channel back to the default theme over its blend-out frames
// Leaf: ?ResetMeleeCombatAbilityEffects@CCombatActionBase@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859660, 311 bytes (plain `ret`). Ego_r.pdb ?ResetMeleeCombatAbilityEffects@CCombatActionBase@@IAEXXZ.
// Callees: CCreatureActionBase::PeekDefinitionManager 0x00692910 / CDefinitionManager::GetDef<CMeleeCombatKnockdownEffects>(long,
//   CDefPointer<const T>*) const 0x0085ABB0 (a same-TU noinline body, the landed 0x008EDFF0 GetDef form), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CVectorMap::LowerBound 0x0040F020 (CThing::GetTC<CTCHaste>, TCI_HASTE 0x8C), CTCHaste::UnsetActionSpeedMultiplier
//   0x0058148D, IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340, CCreatureActionBase::GetWorld 0x00692940 (then the inline
//   CWorld::GetBulletTimeManager / GetEnvironment), CBulletTimeManager::RemoveBulletTimeSlowdownMultiplier 0x004C6140,
//   CEnvironment::TransitionToDefaultThemeAllChannels 0x006B3950; the def's release (slot 1).
// Layout: CMeleeCombatAbilityDef KnockDownEffects +0x60 and CMeleeCombatKnockdownEffects BlendOutFrames +0x40, EnvironmentTheme +0x4C,
//   PauseEntities +0x58 (Ego_r.pdb, retail CDefClassBase 4 bytes smaller and the ValidWeaponClasses vector 12 bytes).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_REMOVESLOWDOWN
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCHaste.h"
#include "engine/CWorld.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 = IS_THING_ALIVE<CIntelligentPointer<CThing> > (Ego_r/FableWin ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z)

class CMeleeCombatKnockdownEffects : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase; Ego_r.pdb members (retail offsets)
    long BlendInFrames;                                     // +0x3c
    long BlendOutFrames;                                    // +0x40
    long AttackerLightingChannel;                           // +0x44
    long TargetLightingChannel;                             // +0x48
    long EnvironmentTheme;                                  // +0x4c
    bool CameraFOVChange;                                   // +0x50
    bool CameraPan;                                         // +0x51
    float AttackerAndTargetSpeedMultiplier;                 // +0x54
    bool PauseEntities;                                     // +0x58
    bool NonAttackingEntities;                              // +0x59
    bool AttackerInvulnerable;                              // +0x5a
};
class CMeleeCombatAbilityDef : public CParentDefClassBase { public:   // RTTI : CParentDefClassBase
    unsigned char _pad3c[0x60 - 0x3c];                      // FlourishAnimName .. ValidWeaponClasses, Decapitate
    long KnockDownEffects;                                  // +0x60
};

#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const   // <CMeleeCombatKnockdownEffects> 0x0085ABB0
{
    if(index<=0) return false;
    CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
    if(!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

void CCombatActionBase::ResetMeleeCombatAbilityEffects()
{
    if (HasResetMeleeCombatAbilityEffects || PMeleeAbilityDef.IsNull() || PMeleeAbilityDef->KnockDownEffects < 0)
        return;
    CDefPointer<const CMeleeCombatKnockdownEffects> effects;
    PeekDefinitionManager().GetDef(PMeleeAbilityDef->KnockDownEffects, &effects);
    if (effects->PauseEntities) {
        CTCHaste* haste;
        if (PCreature->GetTC(&haste))
            haste->UnsetActionSpeedMultiplier(this);
        if (IS_THING_ALIVE(PTarget)) {
            if (PTarget->GetTC(&haste))
                haste->UnsetActionSpeedMultiplier(this);
        }
        GetWorld().GetBulletTimeManager().RemoveBulletTimeSlowdownMultiplier(this);
    }
    if (effects->EnvironmentTheme >= 0)
        GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2, (float)effects->BlendOutFrames);
}
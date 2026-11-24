// CAIStateGroup_Thunder::Update() -- mark the target in danger, and in the idle combat state cast the lightning storm when the target is within the ability range
// Leaf: ?Update@CAIStateGroup_Thunder@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096C4F0, 495 bytes. Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC: CTCBoss 173, CTCStealth 54),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTargetThing +0x1d), CTCStealth::SetAsInDanger 0x00747B80, CTCBoss::GetBossPhase 0x007AED10,
//   CCharString 0x0099EBF0 / 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390,
//   CDefinitionManager::GetDef<CSpecialAbilitiesThunderLightningStormDef> 0x008E31D0, CThing::GetPos 0x004C73D0,
//   CCreatureAction_ThunderLightningStorm ctor 0x0096C6E0, CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70, ~CCombatActionBase
//   0x0062E900, CTCBoss::ResetStrikesSinceHitHero 0x007AED50, CAIStateGroupBase::Terminate 0x008FCF40.
// NF67 (round 7): ported onto the shared headers. The local CThing / CCharString / CSpecialAbilityDef / CDefPointer / CDefinitionManager /
//   CGameDefinitionManager / CTCStealth / CTCThunderCombat / CCreatureActionBase / CAIBrain / CAIStateGroupBase models, the local
//   ETCInterfaceType and the same-TU stand-in GetDef body are gone: the def lookup is the header's FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
//   template (out of line, body visible), interface 0xad is CTCBoss (GetBossPhase 0x007AED10, ResetStrikesSinceHitHero 0x007AED50: nf44:10,
//   nf41:e132), the retail classes come from overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_THUNDER / FABLE_RETAIL_CCREATUREACTION_THUNDERLIGHTNINGSTORM.
//   Distance2D (descriptive, kept from the landed row: no PDB helper with retail's X-then-Y order; GFGetWorld2DDistance takes Y first).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CTCBOSS_STRIKES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_THUNDER
#define FABLE_RETAIL_CCREATUREACTION_THUNDERLIGHTNINGSTORM
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCStealth.h"
#include "engine/CTCBoss.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_ThunderLightningStorm.h"
#include "engine/CAIStateGroup_Thunder.h"
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

FABLE_FORCEINLINE float Distance2D(CThing* target, CThing* me)
{
    const C3DVector& from = me->GetPos();
    const C3DVector& to = target->GetPos();
    float dx = to.X - from.X;
    float dy = to.Y - from.Y;
    return (float)sqrt(dx * dx + dy * dy);
}

void CAIStateGroup_Thunder::Update()
{
    CTCBoss* ptcBoss;
    GetCreature().GetTC(&ptcBoss);
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* stealth;
        if (PTargetThing->GetTC(&stealth))
            stealth->SetAsInDanger(GetCreature(), (NStealth::EDangerStatus)3);
    }
    switch (ptcBoss->GetBossPhase()) {
    case 0:
        if (IS_THING_ALIVE(PTargetThing)) {
            long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", true);
            CDefPointer<const CSpecialAbilitiesThunderLightningStormDef> def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            float dist = Distance2D(PTargetThing.Get(), &GetCreature());
            if (dist < def->EndRadius && dist > 1.0f) {
                EAIActionReturn result = AIAction_PerformActionAndWait(CCreatureAction_ThunderLightningStorm(GetCreature()), false);
                if (result != 0) {
                    HitCount = 0;
                    ptcBoss->ResetStrikesSinceHitHero();
                }
            }
        }
        Terminate();
        break;
    case 1:
        Terminate();
        break;
    }
}

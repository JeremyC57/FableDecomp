// CTCFlammable::FrameUpdate() -- per frame while the thing has a flammable def: fire points whose resistance is used up start burning and set the thing on fire; on catching fire, play the FIRE_DEFAULT sound criteria and attach the fire opinion source once; while on fire (and not damaged per point), every fire-damage period apply the def's per-cycle damage, extinguish when the damage cap is reached or the thing dies (a creature first starts its die action)
// Leaf: ?FrameUpdate@CTCFlammable@@UAEXXZ
// Flags: /d2inlT59
// Inline-budget: /d2inlT59 (EXTRA_RULES 2026-11-08) -- retail calls the CDefString -> CCharString conversion 0x00415DD0 out of line
//   (0x00780E76) while its body is seen: the temporary then shares psound_def's dead frame word (0x00780E6F `lea eax,[esp+14h]`). The
//   default budget expands the conversion and its GetString (545v539); declaration-only conversion keeps the call but homes the
//   temporary in the FIRE_DEFAULT word (539v539, 3 slots). Matching window /d2inlT59..66; the smallest is used.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00780D80, 539 bytes, thiscall, no frame pointer, `ret`. Vtable slot 9 of CTCFlammable.
// Identity: FableWin.pdb ?FrameUpdate@CTCFlammable@@UAEXXZ 0x01F51180; Ego_d.pdb locals it, fpoint, psound_def, ptc_sound_player,
//   pcriteria, this_frame, pthing, damage_this_cycle, extinguished, p_game_object, p_creature.
// Shape: FableWin 0x01F51180 statement for statement (its ASSERTs compiled out; damage_this_cycle's capped value is dead in release):
//   null def -> return; with HasFirePoint, each fire point on fire but not burning is set burning, and any point on fire sets
//   FireResistance -1; on fire and not Burning: Burning, GetTC<CTCSoundPlayer> + GetSubDef<CEntitySoundDef> -> the FIRE_DEFAULT criteria
//   string -> PlayAttachedCriteria(.., false, false, false, -1), then AttachOpinionSource(*ParentThing, NFire::OPINION_SOURCE_DEF_NAME, 0);
//   on fire, not DamagePerPoint and past FireDamageAfterFrame: for a game object, extinguish at the cap, ModifyHealth(FireDamagePerCycle,
//   true), and at (near) zero health a creature's CCreatureAction_Die(*creature, false) and the extinguish; then the next damage frame.
// Callees: NFire::CFirePoint::SetAsBurning 0x00780540, CVectorMap lower bound 0x0040F020 (GetTC), GetSubDef<CEntitySoundDef> 0x0065F604,
//   CCharString 0x0099EBF0 / 0x0099EAE0, CSoundMap::GetPCriteriaString 0x0069D090, CDefString -> CCharString 0x00415DD0,
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CTCBase::PeekWorld 0x00686A80, COpinionReactionManager::AttachOpinionSource 0x00700780,
//   CWorld::GetFrame 0x0049D870, ExtinguishFire 0x00780C80, CThingPhysical::ModifyHealth (slot 59), CCreatureAction_Die 0x008ED650 /
//   ~CCreatureActionBase 0x00693EF0, CThingCreatureBase::SetCurrentAction 0x006644F0, _ftol 0x00BFEA70.
// Globals: NFire::OPINION_SOURCE_DEF_NAME 0x013BADE0 (const CCharString&), DELTA_NOTIONAL_ZERO 0x0129BA3C, the constant FPS 0x01375550.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_DIE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CSOUNDMAP
#define FABLE_RETAIL_CENTITYSOUNDDEF_PASS22_SOUNDMAP
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY   // NF357: GetSubDef<CEntitySoundDef> pick-any copy = size-mode owner 0x0065F604 (scaffolding, not retail)
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTACHOPINIONSOURCE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
#define FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefString.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_Die.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundMap.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/NFire_CFirePoint.h"
#include "engine/CFlammableDef.h"
#include "engine/CTCFlammable.h"

extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB
namespace NFire { extern const CCharString& OPINION_SOURCE_DEF_NAME; }  // 0x013BADE0 ?OPINION_SOURCE_DEF_NAME@NFire@@3ABVCCharString@@B

void CTCFlammable::FrameUpdate()
{
    if (PDef.IsNull())
        return;
    if (HasFirePoint) {
        for (std::vector<NFire::CFirePoint>::iterator it = FirePoints.begin(); it != FirePoints.end(); ++it) {
            NFire::CFirePoint& fpoint = *it;
            if (fpoint.IsOnFire() && !fpoint.IsBurning())
                fpoint.SetAsBurning();
            if (fpoint.IsOnFire())
                FireResistance = -1;
        }
    }
    if (IsOnFire() && !Burning) {
        Burning = true;
        CDefPointer<const CEntitySoundDef> psound_def;
        CTCSoundPlayer* ptc_sound_player;
        if (GetParentThing().GetTC(&ptc_sound_player) && GetParentThing().PeekBaseDef().GetSubDef(&psound_def)) {
            const CDefString* pcriteria = psound_def->SoundMap.GetPCriteriaString("FIRE_DEFAULT");
            if (pcriteria)
                ptc_sound_player->PlayAttachedCriteria(*pcriteria, false, false, false, -1);
        }
        PeekWorld().PeekOpinionReactionManager().AttachOpinionSource(*ParentThing, NFire::OPINION_SOURCE_DEF_NAME, 0);
    }
    if (IsOnFire() && !DamagePerPoint) {
        long this_frame = CWorld::GetFrame();
        if (this_frame > FireDamageAfterFrame) {
            CThing* pthing = &GetParentThing();
            if (pthing->GetTypeFlags() & 4) {
                bool extinguished = false;
                float damage_this_cycle = PDef->FireDamagePerCycle;
                if (fabsf(DamageCaused + damage_this_cycle) >= PDef->MaxDamageCaused) {
                    damage_this_cycle = -PDef->MaxDamageCaused - DamageCaused;
                    ExtinguishFire();
                    extinguished = true;
                }
                DamageCaused += PDef->FireDamagePerCycle;
                CThingGameObject* p_game_object = (CThingGameObject*)pthing;
                p_game_object->ModifyHealth(PDef->FireDamagePerCycle, true);
                if (p_game_object->GetHealth() < DELTA_NOTIONAL_ZERO) {
                    if (p_game_object->GetTypeFlags() & 8) {
                        CThingCreatureBase* p_creature = (CThingCreatureBase*)pthing;
                        p_creature->SetCurrentAction(CCreatureAction_Die(*p_creature, false));
                    }
                    if (!extinguished)
                        ExtinguishFire();
                }
            }
            FireDamageAfterFrame = (long)(this_frame + PDef->FireDamagePeriod * CMainGameComponent::GetConstantFPS());
        }
    }
}
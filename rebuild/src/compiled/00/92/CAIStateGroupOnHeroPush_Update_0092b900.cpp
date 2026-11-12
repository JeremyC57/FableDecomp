// CAIStateGroup_OnHeroPush::Update() -- take the push direction from the creature's push component, normalise it, face along it (pushed from behind) or against it (from the front), play PUSHED_FORWARD / PUSHED_BACKWARD on the first frames of the push, post the hero's push deed, drop the push component and terminate
// Leaf: ?Update@CAIStateGroup_OnHeroPush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092B900, 827 bytes, thiscall virtual, plain `ret`.
// Name: Ego_r.pdb ?Update@CAIStateGroup_OnHeroPush@@UAEXXZ (members PushDir +0x14; CTCOnHeroPush PushDir +0x10 / FrameSincePushed +0x1C).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (PeekTC TCI_ON_HERO_PUSH 196) and non-const
//   0x0040F020 (GetTC TCI_HERO_OPINION_DEED_LOG 81), the physics component's GetRHSet() const (+0x120) and SetFacingAngleXY
//   (+0x108), _CIfmod 0x00BFEB28 (GetAngleXY's wrap), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool, bool, bool, long, long)
//   0x008424F0, ~CCreatureActionBase 0x00693EF0, CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPos 0x004C73D0,
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CThing::RemoveTC 0x004C9840, CAIStateGroupBase::Terminate 0x008FCF40.
// Data: GFastOneOverSqrtTable 0x013CE558 (the expanded Normalise), CMainGameComponent::PMainGameComponent 0x013B86A0, "PUSHED_FORWARD"
//   0x01291BDC, "PUSHED_BACKWARD" 0x01291BCC.
// Shape: PushDir is written and read through one reference local (`direction`), so the facing angle reads X and Y through the
//   same esi base as the Normalise (0x0092BA2C `fld [esi]`, 0x0092BA2E `fld [esi+4]`; the member spelling folds Y to [edi+18h]);
//   the push-vs-facing test is the grouped Dot (Y*Y, Z*Z, then X*X: 0x0092BA0B..0x0092BA1D, header opt-in
//   FABLE_RETAIL_C3DVECTOR_DOT_GROUPED); the Normalise reads the components into locals and sums Z, X, Y (0x0092B95F..0x0092B97A,
//   header opt-in FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS, new in this overlay); the two animation calls differ only in the name
//   and VC7.1 tail-merges them (0x0092BB54). The deed is OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION (73, `push 49h`,
//   0x0092BBFA; engine/EOpinionDeedType.h PDB values); post type 1 stays a number (EOpinionPostDeedType has no shared enumerators).
// Headers: engine/CAIStateGroup_OnHeroPush.h and engine/CTCOnHeroPush.h are new in this overlay (Ego_r.pdb layouts); the action
//   temporary is destroyed through ~CCreatureActionBase 0x00693EF0 directly (implicit destructor, header opt-in
//   FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR, new in this overlay).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCOnHeroPush.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_OnHeroPush.h"

void CAIStateGroup_OnHeroPush::Update()
{
    const CTCOnHeroPush* push;
    if (!GetCreature().PeekTC(TCI_ON_HERO_PUSH, &push))
        return;
    {
        C3DVector& direction = PushDir;
        direction = push->PushDir;
        direction.Normalise();
        CThingAICreature& creature = GetCreature();
        const C3DVector& facing = creature.GetPhysicsTC()->GetRHSet().GetForward();
        C3DVector forward(facing.X, facing.Y, 0.0f);
        CTCPhysicsBase* physics = creature.GetPhysicsTC();
        if (physics) {
            bool fromBehind;
            if (forward.Dot(direction) >= 0.0f) {
                physics->SetFacingAngleXY(direction.GetAngleXY());
                fromBehind = true;
            } else {
                physics->SetFacingAngleXY((-direction).GetAngleXY());
                fromBehind = false;
            }
            if (push->FrameSincePushed <= 1) {
                if (fromBehind)
                    AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "PUSHED_FORWARD", CSoundPair(),
                        false, true, false, 0, 0), false);
                else
                    AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "PUSHED_BACKWARD", CSoundPair(),
                        false, true, false, 0, 0), false);
            }
            CThingPlayerCreature* hero =
                CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroOpinionDeedLog* deeds;
            if (hero && !hero->IsDead() && hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &deeds))
                deeds->PostDeed(OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION, hero->GetPos(), 0, &GetCreature(), (EOpinionPostDeedType)1, false, false, 0,
                    false, 0);
            GetCreature().RemoveTC(TCI_ON_HERO_PUSH);
            Terminate();
            return;
        }
    }
    Terminate();
}
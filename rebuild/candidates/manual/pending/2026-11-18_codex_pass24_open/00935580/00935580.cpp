// CAIStateGroup_MinionFaceHero::IsRunnable() -- check whether the minion can turn to face the hero
// Grade: DIFFER(605v617) -- The shared port preserves original fields and call names but changes the minion state and vector frame homes. The retired local-model draft had a smaller x87 residue.
// Leaf: ?IsRunnable@CAIStateGroup_MinionFaceHero@@UAE_NXZ
// Retail: 0x00935580, 617 bytes, virtual thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_DEFAULT_SQUARE
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/CAIBrain.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroup_MinionFaceHero.h"

bool FABLE_FASTCALL GFWorld3DDistanceSmallerThan(const C3DVector&, const C3DVector&, float);

bool CAIStateGroup_MinionFaceHero::IsRunnable()
{
    const CThingPlayerCreature* phero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (phero && !phero->IsDead()) {
        const CTCPerceiveThing* tc_perception;
        bool face_hero = false;
        if (PBrain->Creature->PeekTC(TCI_PERCEIVE_THING, &tc_perception)
                && tc_perception && tc_perception->IsPlayerExtendedLineOfSight()
                && (FrameLastLookedAtHero == 0 || static_cast<unsigned long>(CWorld::GetFrame()) - FrameLastLookedAtHero
                            > static_cast<unsigned long>(CMainGameComponent::GetConstantFPS() * 35))
                && GFWorld3DDistanceSmallerThan(PBrain->Creature->GetPos(), phero->GetPos(), 2.2f)) {
            C3DVector vec_to_face_me = PBrain->Creature->PeekPos() - phero->PeekPos();
            vec_to_face_me.Z = 0.0f;
            vec_to_face_me.Normalise();
            C3DVector hero_facing_vec = phero->GetPhysicsTC()->GetFacingVector();
            hero_facing_vec.Z = 0.0f;
            hero_facing_vec.Normalise();
            face_hero = hero_facing_vec.Dot(vec_to_face_me) > 0.4f;
            if (face_hero) {
                Hero = phero;
                return true;
            }
        }
    }
    return false;
}

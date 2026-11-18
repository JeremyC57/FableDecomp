// CTCHitOnCollision::OnCollision(const C3DVector& collide_pos, const C3DVector& collision_normal, CThing& target) -- hit a creature the owner collides with once: skip it while it performs a combat action when so set, optionally push it sideways from the owner's facing, apply the damage, and unless the hit was blocked or negated (or pass-through is set) remember it and make the owner not solid to it
// Grade: DIFFER(768v768) -- The iterator tag and hit_direction stack positions differ from 0x007C8210.
// Leaf: ?OnCollision@CTCHitOnCollision@@QAEXABVC3DVector@@0AAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C81F0, 768 bytes, `ret 0Ch`. Ego_r.pdb / FableWin.pdb ?OnCollision@CTCHitOnCollision@@QAEXABVC3DVector@@0AAVCThing@@@Z;
//   Ego_d.pdb tc_hit_on_collision.obj (locals targetGameObject / doHit / blocked / creature / pAction / hit_pos / hit_direction / params /
//   vector_to_target / left / pTCSolidToThing and their scopes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_SETWEAPON
#define FABLE_RETAIL_CHITPARAMETERS_SETBLOCKABLE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CHitParameters.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCHitOnCollision.h"
#include "stlport/fable_algorithm.h"

void CTCHitOnCollision::OnCollision(const C3DVector& collide_pos, const C3DVector& collision_normal, CThing& target)
{
    if (target.GetTypeFlags() & 8) {
        CThingGameObject& targetGameObject = static_cast<CThingGameObject&>(target);
        if (std::find(CollidedList.begin(), CollidedList.end(), &targetGameObject) == CollidedList.end()) {
            bool doHit = true;
            if (DontHitWhenCombatAction) {
                CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(target);
                const CCreatureActionBase* pAction = creature.PeekPCurrentAction();
                if (pAction && pAction->IsStrikeAction())
                    doHit = false;
            }
            if (doHit) {
                C3DVector hit_direction = collision_normal;
                C3DVector hit_pos = collide_pos;
                if (PushSideways) {
                    C3DVector left = GetParentThing().GetPhysicsTC()->GetFacingVector();
                    left.RotateXY(-0.25f);
                    C3DVector vector_to_target = target.GetPos() - GetParentThing().GetPos();
                    vector_to_target.Normalise();
                    hit_direction = left;
                    if (left.Dot(vector_to_target) > 0.0f)
                        hit_direction.RotateXY(0.5f);
                    hit_pos = hit_direction + target.GetPos();
                }
                CHitParameters params(0, &GetParentThing(), 0, hit_pos, hit_direction, true, false, false, false);
                params.AddDamageWithAttribute((EDamageAttribute)0, Damage);
                params.SetBlockable(Blockable);
                params.SetWeapon(GetParentThing());
                targetGameObject.OnHit(params);
                if (PassThroughIfBlocked || (!params.IsBlocked() && !params.IsHitNegated())) {
                    CollidedList.push_back(&targetGameObject);
                    if (!GetParentThing().HasTC(TCI_SOLID_TO_THING))
                        GetParentThing().AddTC("CTCSolidToThing", false, 0);
                    CTCSolidToThing* pTCSolidToThing;
                    if (GetParentThing().GetTC(&pTCSolidToThing))
                        pTCSolidToThing->AddThingNotSolidTo(&target);
                }
            }
        }
    }
}

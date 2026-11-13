// CTCLightningOrb::Initialise(CThing& parent, CThing& target) -- launch the orb from its thing at the target: remember parent and target, predict the target's ground position, build the flight spline (start, a raised take-off point, a randomly bowed mid point, two approach points for the Fireheart, the target), set the traversal frame count from the def speed, attach a lightning node (parent as faction and hit source) when the def names lightning, switch the orb's physics to weightless moving, force-resend its graphics and attach the LIGHTNING_ORB particle effect
// Leaf: ?Initialise@CTCLightningOrb@@QAEXAAVCThing@@0@Z
// Retail: Fable.exe 0x007F26B0, 1802 bytes (`sub esp,8Ch` .. `ret 8` at 0x007F2DB7; CTCLightningNode::FrameUpdate follows at 0x007F2DC0).
//   Ego_r.pdb / FableWin.pdb ?Initialise@CTCLightningOrb@@QAEXAAVCThing@@0@Z (public, non-virtual).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (the inline SetPParent /
//   SetPTarget and IS_THING_ALIVE); CThing::GetPos 0x004C73D0; CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480;
//   CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810, GFFloatRandom 0x004978A0; C3DVector::Rotate(float, float) 0x00A14260,
//   AccurateNormalise 0x00A14440; CThing::GetDefName 0x004C7CC0, CDefString::operator==(const char*) 0x005B3440
//   ("OBJECT_LPDD_FIREHEART_01" 0x01271D6C); operator new 0x00BFEA1A (0x18 bytes, the inline CLinearSpline ctor); CCountedPointer<
//   NBBBMaths::CLinearSpline>::Reset 0x007F4780; NBBBMaths::CLinearSpline::AddControlPoint 0x00A580F0 / ProcessPoints(long) 0x00A58130;
//   __ftol2 0x00BFEA70; CThing::AddTC 0x004C9D60 ("CTCLightningNode" 0x0123E650); CTCLightningNode::SetPParent 0x007F39A0 /
//   SetPSourceOfFaction 0x007F39F0 / SetPSourceOfHit 0x007F3A40 (called), SetPDef (expanded); CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetDef<CLightningDef> 0x007F3BB0; CTCPhysicsBase slot 38 EnableGravity(false), SetAsMoving 0x006B0130;
//   GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30 ("LIGHTNING_ORB" 0x01271D5C);
//   CTCDParticleEmitter::Create 0x006E0880, AttachToThing 0x006E0BE0; CCharString 0x0099EBF0 / 0x0099E4B0 / ~ 0x0099EAE0.
// Shape (frame layout from the c2 frame-packer trace, 0x8C as retail): the target velocity is a named C3DVector `(position - last
//   position) * 0.0f * GetConstantFPS()`; the difference to the target is normalised IN PLACE and then copied into `direction`
//   (`difference /= distance; C3DVector direction = difference;`): the copy is forwarded, so one node holds difference, direction,
//   velocity and two later temporaries ([esp+18h], the difference's Z at [esp+20h], 0x007F27FE), and the copy keeps retail's X, Z, Y
//   squares and stored reciprocal (0x007F2802..0x007F2840; a plain in-place `direction /= distance` squares X, Y, Z, and dividing a
//   copy makes difference and direction conflict, so difference took a slot of the 8-byte group); the two Fireheart approach points are
//   named block-scoped vectors (their addresses go to AddControlPoint, so each lives to the end of the block: they open retail's two
//   temporary slots [esp+60h] / [esp+3Ch] and push the second point's flat_direction * 7 product into its own slot [esp+90h]); the frame
//   count `(distance / Speed) * GetConstantFPS()` parenthesised (fimul, 0x007F2BBE); the particle effect's helper name a named
//   CCharString (built before the pushes, 0x007F2D67); GetEffectFromString's body visible in the TU
//   (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY): `effect` is then not captured, so it shares [esp+10h] with the GetTC key as in
//   retail (0x007F2CEB / 0x007F2D43). The parameter homes stay unused because the packed frame is >= 0x70 bytes (c2.dll 0x10734A51
//   repacks without them), as retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME

#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CDefString.h"
#include "engine/CTCLightningNode.h"
#include "engine/CTCLightningOrb.h"
class CLightningDef;

void CTCLightningOrb::Initialise(CThing& parent, CThing& target)
{
    SetPParent(parent);
    SetPTarget(target);
    CTCPhysicsBase* target_physics = target.GetPhysicsTC();
    C3DVector velocity = (target_physics->GetPosition() - target_physics->GetLastPosition()) * 0.0f
                         * CMainGameComponent::GetConstantFPS();
    C3DVector target_pos = target.GetPos() + velocity;
    target_pos.Z = GetWorldMap().GetHeightZBelowPos(target_pos);
    C3DVector start = ParentThing->GetPos();
    C3DVector difference = target_pos - start;
    float distance = difference.GetMagnitude();
    difference /= distance;
    C3DVector direction = difference;
    C3DVector middle = direction * distance * 0.5f + start;
    C3DVector up = direction ^ direction.CrossWithUp();
    up.Rotate(distance, 0.06f - GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) * 0.12f);
    middle -= up * distance * 0.075f;
    bool fireheart = false;
    float approach_distance;
    float approach_height;
    if (IS_THING_ALIVE(PTarget) && PTarget->GetDefName() == "OBJECT_LPDD_FIREHEART_01") {
        fireheart = true;
        approach_distance = 5.0f;
        approach_height = 1.25f;
    } else {
        approach_distance = 2.0f;
        approach_height = 0.5f;
    }
    C3DVector flat_direction(direction.X, direction.Y, 0.0f);
    flat_direction.AccurateNormalise();
    C3DVector approach = start + flat_direction * approach_distance + C3DVector(0.0f, 0.0f, approach_height);
    PSpline.Reset(new NBBBMaths::CLinearSpline);
    PSpline->AddControlPoint(start);
    PSpline->AddControlPoint(approach);
    PSpline->AddControlPoint(middle);
    if (fireheart) {
        C3DVector fireheart_approach = target_pos - flat_direction * 7.0f;
        PSpline->AddControlPoint(fireheart_approach);
        C3DVector fireheart_rise = target_pos - flat_direction * 7.0f * 0.5f + C3DVector(0.0f, 0.0f, 2.0f);
        PSpline->AddControlPoint(fireheart_rise);
    }
    PSpline->AddControlPoint(target_pos);
    if (PDef->Speed > DELTA_NOTIONAL_ZERO)
        NumFramesForSplineTraversal = (long)((distance / PDef->Speed) * CMainGameComponent::GetConstantFPS());
    if (PDef->LightningDefIndex > 0) {
        CTCLightningNode* node = static_cast<CTCLightningNode*>(ParentThing->AddTC("CTCLightningNode", false, 0));
        node->SetPParent(*ParentThing);
        node->SetPSourceOfFaction(&parent);
        node->SetPSourceOfHit(&parent);
        CDefPointer<const CLightningDef> lightning_def;
        GetDefinitionManager().GetDef(PDef->LightningDefIndex, &lightning_def);
        node->SetPDef(lightning_def);
    }
    ParentThing->GetPhysicsTC()->EnableGravity(false);
    ParentThing->GetPhysicsTC()->SetAsMoving(true);
    ParentThing->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
    CDefPointer<const CSpecialEffectsDef> special_effects;
    if (ParentThing->GetPBaseDef()->GetSubDef(&special_effects)) {
        long effect;
        if (special_effects->GetEffectFromString("LIGHTNING_ORB", effect)) {
            CThing* effect_thing = CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false);
            if (IS_THING_ALIVE(effect_thing)) {
                CTCDParticleEmitter* emitter;
                effect_thing->GetTC(&emitter);
                CCharString helper_name;
                emitter->AttachToThing(*ParentThing, 8, helper_name, 0, 0.0f);
            }
        }
    }
    PSpline->ProcessPoints(1);
}

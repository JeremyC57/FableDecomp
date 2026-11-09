// CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe() -- true when the target scorpion is alive, charging (FindChargeEvent) and its normalised facing points within 45 degrees of the normalised vector from it to this creature
// Leaf: ?IsScorpionChargingAtMe@CAIStateGroup_VersusScorpionKing@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00963880, 463 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb ?IsScorpionChargingAtMe@CAIStateGroup_VersusScorpionKing@@AAE_NXZ (0x00988150), right after
//   ?FindChargeEvent@..@@AAE_NXZ (0x009880C0) as retail places the callee 0x00963810 right before this body.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x4 on PTargetThing +0x14), FindChargeEvent 0x00963810, the physics TC's
//   virtual GetFacingVector() const (slot 70, +0x118, hidden result), CThing::GetPos 0x004C73D0 (x2).
// Data: GFastOneOverSqrtTable 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.707f (the 45-degree cosine).
// Shape: from FindChargeEvent's test to the return the body is one block of 120 IL tuples, so VC7.1's list scheduler cuts it at
//   81 tuples (c2.dll 0x107B4E6A), counting the no-code PAREN tuples of parenthesised float operands. Retail's cut falls inside
//   the second (toMe) Normalise's integer estimate, after `shr ecx,17h` (0x009639B3): `and ecx,0FFh` then follows `shr eax,11h`
//   and `mov edx,17Ch` the table load (0x009639C9), and the first estimate's shifts interleave (0x0096392F..0x0096394D). That
//   needs four more PAREN tuples before the cut than `creature->GetPos() - target->GetPos()` and the default GFFastOneOverSqrt
//   give: the three parenthesised components of toMe (written out, target position fetched first as retail calls it,
//   0x009638DB..0x009638EB) and the parenthesised Newton product of the first GFFastOneOverSqrt
//   (FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN, which 0x0096E830 needs as well). With three (md 2) the cut is one tuple
//   late, with five (md 2) one early, with none (md 22) `mov edx,17Ch` hoists into the first Normalise. A parenthesised
//   C3DVector::operator- would give the same three here but breaks 0x0096E830's `enemyPos - preferred` (md 6), so the
//   header operator- stays unparenthesised. GetSquaredMagnitude in X, Y, Z order (FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ)
//   loads facing Y first as retail does (0x00963909).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUP_VERSUSENEMYBASE
#define FABLE_RETAIL_CAISTATEGROUP_VERSUSSCORPIONKING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_VersusEnemyBase.h"
#include "engine/CAIStateGroup_VersusScorpionKing.h"

template<class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() != 0 && !p.Get()->IsDead(); }

bool CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe()
{
    if (IS_THING_ALIVE(PTargetThing) && FindChargeEvent()) {
        C3DVector facing = PTargetThing->GetPhysicsTC()->GetFacingVector();
        CThingAICreature* creature = PBrain->Creature;
        const C3DVector& targetPos = PTargetThing->GetPos();
        const C3DVector& myPos = creature->GetPos();
        C3DVector toMe((myPos.X - targetPos.X), (myPos.Y - targetPos.Y), (myPos.Z - targetPos.Z));
        facing.Normalise();
        toMe.Normalise();
        if (facing.Dot(toMe) > 0.707f)
            return true;
    }
    return false;
}
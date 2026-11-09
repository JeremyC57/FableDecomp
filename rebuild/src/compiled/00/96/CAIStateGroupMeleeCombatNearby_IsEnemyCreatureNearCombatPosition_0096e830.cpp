// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition() const -- for a creature in melee as an attacker: true when its preferred combat position is within (twice its physics radius x 5/6 + 2.8) and the planar directions preferred->enemy and creature->enemy lie within 85 degrees; otherwise falls back to IsEnemyCreatureInCombatPosition()
// Leaf: ?IsEnemyCreatureNearCombatPosition@CAIStateGroup_MeleeCombatNearby@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096E830, 544 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb ?IsEnemyCreatureNearCombatPosition@CAIStateGroup_MeleeCombatNearby@@ABE_NXZ (0x00993440, private const),
//   right after ?IsEnemyCreatureInCombatPosition@..@@ABE_NXZ (0x009932B0) as retail places the landed 0x0096E700 before it.
// Callees: CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 (the landed sibling's spelling; retail order after
//   IsFreeToPerformAction 0x00662210 as Ego_r 0x0053D2A0 after 0x0053D090), CAIStateGroupBase::GetWorld 0x008FCFC0 (twice inline
//   in GetCombatManager: Ego_r ?GetCombatManager@CAIStateGroup_MeleeCombatNearby@@ABEAAVCCombatManager@@XZ + FableWin
//   ?GetCombatManager@CWorld@@QAEAAVCCombatManager@@XZ), CCombatManager::GetPreferredPos 0x006EA770, CThing::GetPos 0x004C73D0 (x4),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2 on PEnemy +0x14), IsEnemyCreatureInCombatPosition 0x0096E700.
// Data: GFastOneOverSqrtTable 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 0.8333333f 0x012925CC (the landed
//   sibling's reach scale), 2.8f 0x01297070, and cos's argument 0x01297068 = (double)(85/360 turns as a float) * TWO_PI
//   (1.4835298638662664: GFCos(float turns) with the ?TWO_PI@@3NB const double, folded).
// Shape: CAIStateGroup_MeleeCombatNearby::GetCombatManager (inline) evaluates GetWorld before the GetPreferredPos pushes, with the
//   creature loaded first into edi (0x0096E84F; a direct `GetWorld().PThingCombatManager_ptr->` pushes first). The planar
//   directions are `a - b` with Z then zeroed (an explicit (x, y, 0.0f) constructor leaves toEnemy.Z live on the x87 stack and
//   reorders the dot product, 552 v 544 bytes). The block after the combatant test is 140 IL tuples, cut at VC7.1's 81-tuple
//   region cap (c2.dll 0x107B4E6A); retail's cut falls before toEnemy's Y `fsub` (0x0096E94E), so `mov ecx,17Ch` and two pops
//   fill the fsub latency ahead of `fld st(0)` (0x0096E951..0x0096E958). That needs one PAREN tuple more than the default header
//   gives before it: the parenthesised Newton product of the first GFFastOneOverSqrt (FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN,
//   as 0x00963880). Parenthesised operator- arguments instead (+2 per vector) put the cut 3 tuples early (md 6).
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
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBYBASE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_COMBATMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#include <math.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"
#include "engine/CAIStateGroup_MeleeCombatNearby.h"

const double TWO_PI = 6.283185482025146484375;          // 0x0124F2B8 ?TWO_PI@@3NB (internal-linkage const: folded here)
inline double FABLE_FASTCALL GFCos(float angle) { return cos(angle * TWO_PI); }   // angle in turns

bool CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition() const
{
    if (GetCreature().IsInMeleeAsAttacker()) {
        C3DVector preferred = GetCombatManager().GetPreferredPos(GetCreature());
        const C3DVector& here = GetCreature().GetPos();
        C3DVector toPreferred = preferred - here;
        const C3DVector& enemyPos = PEnemy->GetPos();
        C3DVector fromPreferred = enemyPos - preferred;
        fromPreferred.Z = 0.0f;
        fromPreferred.Normalise();
        const CThingAICreature& creature = GetCreature();
        const CThingCreatureBase& enemy = *PEnemy.Get();
        const C3DVector& from = creature.GetPos();
        const C3DVector& to = enemy.GetPos();
        C3DVector toEnemy = to - from;
        toEnemy.Z = 0.0f;
        toEnemy.Normalise();
        float range = (GetCreature().GetPhysicsTC()->GetRadius() + GetCreature().GetPhysicsTC()->GetRadius()) * 0.8333333f + 2.8f;
        if (toPreferred.X * toPreferred.X + toPreferred.Y * toPreferred.Y < range * range && toEnemy.Dot(fromPreferred) > GFCos(85.0f / 360.0f))
            return true;
        if (IsEnemyCreatureInCombatPosition())
            return true;
    }
    return false;
}
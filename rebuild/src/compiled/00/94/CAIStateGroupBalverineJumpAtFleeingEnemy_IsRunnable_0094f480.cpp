// CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable() -- the balverine may jump at its best enemy when that enemy is alive, known to it, moving away fast enough along the balverine's facing, and between 3 and 5.5 units away (2D); the enemy becomes PTarget.
// Leaf: ?IsRunnable@CAIStateGroup_BalverineJumpAtFleeingEnemy@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094F480, 437 bytes, thiscall, no arguments, plain `ret`, FPO (ebp = this), no SEH.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?IsRunnable@CAIStateGroup_BalverineJumpAtFleeingEnemy@@MAE_NXZ (protected virtual);
//   vtable 0x0128C0F8 slot 11 (rebuild/manifest/rtti-vtables.tsv) is its only reference.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (non-const, inside the expanded CThing::GetTC),
//   CTCTargetingBase virtual slot 29 (+0x74) GetPBestEnemyCreatureForAttack() const (the second new virtual of CTCTargetingBase:
//   slots 28 / 30 of the CTCTargetingAI vtable are the landed GetPBestThingForMeleeAttack / GetPBestThingForProjectileTargetingLock,
//   and it returns the CThingCreatureBase* that PTarget and GetMaxRunningSpeed need without a cast),
//   NCreatureAI::IsAwareOfThing 0x008645E0 (__fastcall), CTCPhysicsBase virtual slot 70 (+0x118) GetFacingVector() const (x3),
//   CThing::GetPos 0x004C73D0, CThingCreatureBase virtual slot 81 (+0x144) GetMaxRunningSpeed(const C3DVector&) const,
//   CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(T*) 0x00668B60, ::operator= 0x00668B80,
//   CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10.
// Shape: one nested condition with a single `return false` (every early-out jumps to the one false epilogue at 0x0094F62C, and
//   esi / edi are pushed before the first test); the creature is read through the inline CAIStateGroupBase::GetCreature()
//   (PBrain->GetCreature()) at every use and never kept in a source local -- retail reloads [this+4]->+0x18 for GetTC,
//   IsAwareOfThing, the facing and the offset to the enemy, and only the `GetPos() + GetFacingVector()` statement keeps it in ebx
//   across the virtual call (0x0094F54B .. 0x0094F56F); a `CThingAICreature*` local, or PBrain->Creature spelled directly, lets
//   VC7.1 share one load with the previous statement instead. The enemy's facing vector is fetched and never used (retail keeps
//   the call, result at esp+0x3C). The offset to the enemy only keeps X and Y (GetSquaredMagnitude2D); the default C3DVector
//   branch (one-line `X*X+Y*Y`) loads Y first as retail does. `PTarget = enemy` is the out-of-line (T*) constructor, the
//   out-of-line operator= and the base destructor of the temporary.
// Headers: overlay opt-in FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY (the real class over CAIStateGroupBase).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_BALVERINEJUMPATFLEEINGENEMY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"

namespace NCreatureAI {
    bool __fastcall IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool direct);   // 0x008645E0
}

bool CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable()
{
    CTCTargetingBase* targeting;
    if (GetCreature().GetTC(TCI_TARGETING, &targeting)) {
        CThingCreatureBase* enemy = targeting->GetPBestEnemyCreatureForAttack();
        if (enemy && !enemy->IsDead() && NCreatureAI::IsAwareOfThing(&GetCreature(), enemy, false)) {
            const CTCPhysicsBase* enemyPhysics = enemy->GetPhysicsTC();
            C3DVector enemyMovement = enemyPhysics->GetPosition() - enemyPhysics->GetLastPosition();
            C3DVector enemyFacing = enemyPhysics->GetFacingVector();
            C3DVector facing = GetCreature().GetPhysicsTC()->GetFacingVector();
            C3DVector toEnemy = enemyPhysics->GetPosition() - GetCreature().GetPhysicsTC()->GetPosition();
            C3DVector ahead = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector();
            float approach = facing.Dot(enemyMovement);
            if (approach > enemy->GetMaxRunningSpeed(ahead) * 0.5f) {
                float distanceSquared = toEnemy.GetSquaredMagnitude2D();
                if (distanceSquared < 30.25f && distanceSquared > 9.0f) {
                    PTarget = enemy;
                    return true;
                }
            }
        }
    }
    return false;
}
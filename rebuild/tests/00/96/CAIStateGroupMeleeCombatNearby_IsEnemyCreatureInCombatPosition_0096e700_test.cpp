// Behaviour test for retail 0x0096e700: the candidate source below is embedded verbatim (from src/0096e700.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() -- true when the creature already stands within reach
//   of its preferred combat position, or, if that position is not taken, within reach of the enemy.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096E700, 297 bytes.
// Callees: the creature's combatant test 0x00662410 (forwards to the world combat manager's 0x006E92C0; manifest label
//   propagated), CAIStateGroupBase::GetPWorld 0x008FCFC0 (this TU's copy of PBrain->+0x1c->+0x24; 0x008FCFA0 is the other),
//   CCombatManager::GetPreferredPos(CThingCreatureBase*) 0x006EA770 (manifest), CCombatManager position-occupied test
//   0x006E9700 (autoname label only), CThing::GetPosition 0x004C73D0 (physics +0x60, position +0x0c),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Constants: four const float globals in .rdata (separate operands: VC would fold literal 0.3f + 1e-4f into one):
//   ReachScale 0.8333333 (0x012925CC), PreferredPosMargin 0.3 (0x01238170), ReachEpsilon 1e-4 (0x0129BA3C),
//   EnemyMargin 1.0 (0x0122DED8); reach = (r + r) * ReachScale + margin + ReachEpsilon. Names are descriptive.
// Layout: engine/CAIStateGroupBase.h (PBrain +0x04), CAIBrain.h (Creature +0x18), CWorld.h (PThingCombatManager_ptr
//   +0x4c); the group's enemy pointer at +0x14 (PDB: unnamed base bytes) and the physics radius at +0x34 are retail
//   offsets. The *_Methods structs are the lane's method carriers for generated data-only headers.
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
struct C3DVector { float X, Y, Z; };
extern const float ReachScale;                          // 0x012925CC = 0.8333333
extern const float PreferredPosMargin;                  // 0x01238170 = 0.3
extern const float ReachEpsilon;                        // 0x0129BA3C = 1e-4
extern const float EnemyMargin;                         // 0x0122DED8 = 1.0
struct CBaseObject;
struct CThingPhysics { char _prefix[0x34]; float Radius; };                  // +0x34
struct CThingCreatureBase {
    char _prefix[0x60]; CThingPhysics* Physics;                             // +0x60
    bool IsAttackingCombatant();                        // 0x00662410
    const C3DVector& GetPosition();                     // 0x004C73D0 (CThing::GetPosition)
};
struct CBaseIntelligentPointer { void* __vftable; void* PData; CBaseObject* GetPItem() const; };   // 0x00A01B50
struct CCombatManager {
    C3DVector GetPreferredPos(CThingCreatureBase* creature);                // 0x006EA770
    bool IsPositionOccupied(CThingCreatureBase* creature);                  // 0x006E9700
};
struct CAIStateGroupBase_Methods : CAIStateGroupBase {
    CWorld* GetPWorld_008fcfc0();                                // 0x008FCFC0
};
struct CAIStateGroup_MeleeCombatNearby_Methods : CAIStateGroup_MeleeCombatNearby {
    __forceinline CThingCreatureBase* Creature() { return (CThingCreatureBase*)((CAIStateGroupBase*)this)->PBrain->Creature; }
    __forceinline CWorld* GetPWorld() { return ((CAIStateGroupBase_Methods*)(CAIStateGroupBase*)this)->GetPWorld_008fcfc0(); }
    __forceinline CBaseIntelligentPointer* PEnemy() { return (CBaseIntelligentPointer*)_pad_0x14; }   // +0x14
    bool IsEnemyCreatureInCombatPosition();
};
bool CAIStateGroup_MeleeCombatNearby_Methods::IsEnemyCreatureInCombatPosition() {
    if (Creature()->IsAttackingCombatant()) {
        CThingCreatureBase* actor = Creature(); CWorld* world = GetPWorld();
        C3DVector preferred = world->PThingCombatManager_ptr->GetPreferredPos(actor);
        const C3DVector& position = Creature()->GetPosition();
        float dx = preferred.X - position.X, dy = preferred.Y - position.Y;
        float reach = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + ReachEpsilon + PreferredPosMargin;   // written e-first: VC7.1 (no /Op) emits these two adds swapped (retail order: 0.3 then 1e-4)
        if (dx * dx + dy * dy < reach * reach) return true;
        const C3DVector& target = ((CThingCreatureBase*)PEnemy()->GetPItem())->GetPosition();
        const C3DVector& current = Creature()->GetPosition();
        C3DVector delta; delta.X = target.X - current.X; delta.Y = target.Y - current.Y; delta.Z = 0;
        CThingCreatureBase* owner = Creature(); CCombatManager* manager = GetPWorld()->PThingCombatManager_ptr;
        if (!manager->IsPositionOccupied(owner)) {
            float limit = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + EnemyMargin + ReachEpsilon;
            if (delta.X * delta.X + delta.Y * delta.Y < limit * limit) return true;
        }
    }
    return false;
}

// ===== harness =====
#include <cstdio>
static int errors,scenario,worldCalls,preferredCalls,occupiedCalls,positionCalls,enemyGets;
static CThingCreatureBase actor,enemy;static CThingPhysics physics;static CCombatManager manager;static CWorld world;
static C3DVector origin={0,0,1000},enemyPosition={0,0,-1000},preferred;
extern const float ReachScale=0.8333333134651184f,PreferredPosMargin=0.3f,ReachEpsilon=0.0001f,EnemyMargin=1.0f;
bool CThingCreatureBase::IsAttackingCombatant(){if(this!=&actor)++errors;return scenario!=0;}
const C3DVector& CThingCreatureBase::GetPosition(){++positionCalls;if(this==&actor)return origin;if(this!=&enemy)++errors;return enemyPosition;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{++enemyGets;return (CBaseObject*)&enemy;}
CWorld* CAIStateGroupBase_Methods::GetPWorld_008fcfc0(){++worldCalls;return &world;}
C3DVector CCombatManager::GetPreferredPos(CThingCreatureBase* p){++preferredCalls;if(this!=&manager||p!=&actor||worldCalls!=1)++errors;return preferred;}
bool CCombatManager::IsPositionOccupied(CThingCreatureBase* p){++occupiedCalls;if(this!=&manager||p!=&actor||worldCalls!=2)++errors;if(scenario==5)physics.Radius=10;return scenario==3;}
int main(){CAIStateGroup_MeleeCombatNearby_Methods g;CAIBrain brain;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;brain.Creature=(CThingAICreature*)&actor;
#else
g.PBrain=&brain;brain.Creature=&actor;
#endif
actor.Physics=&physics;
#ifdef _MSC_VER
world.PThingCombatManager_ptr=(CCombatManager*)&manager;
#else
world.PThingCombatManager_ptr=&manager;
#endif

for(scenario=0;scenario<6;++scenario){worldCalls=preferredCalls=occupiedCalls=positionCalls=enemyGets=0;physics.Radius=1;preferred.X=scenario==1?1:100;preferred.Y=0;preferred.Z=9999;enemyPosition.X=scenario>=4?5:1;bool expected=scenario==1||scenario==2||scenario==5;bool result=g.IsEnemyCreatureInCombatPosition();int later=scenario>=2;
if(result!=expected||worldCalls!=(scenario==0?0:scenario==1?1:2)||preferredCalls!=(scenario!=0)||occupiedCalls!=later||enemyGets!=later||positionCalls!=(scenario==0?0:scenario==1?1:3))++errors;}
if(errors){std::printf("0096e700_TEST FAIL %d\n",errors);return 1;}std::printf("0096e700_TEST PASS\n");return 0;}
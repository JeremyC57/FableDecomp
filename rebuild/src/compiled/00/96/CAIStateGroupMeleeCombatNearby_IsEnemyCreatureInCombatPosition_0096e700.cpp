// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: ReachEpsilon bound to two different float pools (0.3 at 0x1238170, 1e-4 at 0x129ba3c); PreferredMargin/ReachEpsilon swapped)
// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() -- true when the creature already stands within reach
//   of its preferred combat position, or, if that position is not taken, within reach of the enemy.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096E700, 297 bytes.
// Callees: the creature's combatant test 0x00662410 (forwards to the world combat manager's 0x006E92C0; manifest label
//   propagated), CAIStateGroupBase::GetPWorld 0x008FCFC0 (this TU's copy of PBrain->+0x1c->+0x24; 0x008FCFA0 is the other),
//   CCombatManager::GetPreferredPos(CThingCreatureBase*) 0x006EA770 (manifest), CCombatManager position-occupied test
//   0x006E9700 (autoname label only), CThing::GetPos 0x004C73D0 (physics +0x60, position +0x0c),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Constants: four const float globals in .rdata (separate operands: VC would fold literal 0.3f + 1e-4f into one):
//   ReachScale 0.8333333 (0x012925CC), PreferredPosMargin 0.3 (0x01238170), ReachEpsilon 1e-4 (0x0129BA3C),
//   EnemyMargin 1.0 (0x0122DED8); reach = (r + r) * ReachScale + margin + ReachEpsilon. Names are descriptive.
// Layout: engine/CAIStateGroupBase.h (PBrain +0x04), CAIBrain.h (Creature +0x18), CWorld.h (PThingCombatManager_ptr
//   +0x4c); the group's enemy pointer at +0x14 (PDB: unnamed base bytes) and the physics radius at +0x34 are retail
//   offsets. The *_Methods structs are the lane's method carriers for generated data-only headers.
#include "engine/CAIStateGroup_MeleeCombatNearby.h"
#define CAIStateGroupBase CAIStateGroupBasePdb
#include "engine/CAIStateGroupBase.h"              // layout only; the local class below carries the protected accessor (PDB: IBE)
#undef CAIStateGroupBase
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
class CAIStateGroupBase : public CAIStateGroupBasePdb {
protected:
    CWorld& GetWorld() const;                               // 0x008FCFC0 (PDB ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
class C3DVector { public: float X, Y, Z; };
extern const float ReachScale;                          // 0x012925CC = 0.8333333
extern const float PreferredPosMargin;                  // 0x01238170 = 0.3
extern const float ReachEpsilon;                        // 0x0129BA3C = 1e-4
extern const float EnemyMargin;                         // 0x0122DED8 = 1.0
class CBaseObject;
struct CThingPhysics { char _prefix[0x34]; float Radius; };                  // +0x34
class CThing { public:
    const C3DVector& GetPos() const;                     // 0x004C73D0 (CThing::GetPos)
};
class CThingCreatureBase : public CThing { public:
    char _prefix[0x60]; CThingPhysics* Physics;                             // +0x60
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};
struct CBaseIntelligentPointer { void* __vftable; void* PData; CBaseObject* GetPItem() const; };   // 0x00A01B50
class CCombatManager { public:
    C3DVector GetPreferredPos(CThingCreatureBase* creature);                // 0x006EA770
    bool IsPositionOccupied(CThingCreatureBase* creature);                  // 0x006E9700
};
struct CAIStateGroup_MeleeCombatNearby_Methods : CAIStateGroupBase {   // the engine header's flat layout past the base: +0x14 enemy pointer, +0x1c Archer
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    bool          Archer;                                    // +0x1c
    unsigned char _pad_0x1d[0x3];                            // +0x1d
    __forceinline CThingCreatureBase* Creature() { return (CThingCreatureBase*)((CAIStateGroupBase*)this)->PBrain->Creature; }
    __forceinline CWorld* GetPWorld() { return &GetWorld(); }
    __forceinline CBaseIntelligentPointer* PEnemy() { return (CBaseIntelligentPointer*)_pad_0x14; }   // +0x14
    bool IsEnemyCreatureInCombatPosition();
};
bool CAIStateGroup_MeleeCombatNearby_Methods::IsEnemyCreatureInCombatPosition() {
    if (Creature()->IsInMeleeAsAttacker()) {
        CThingCreatureBase* actor = Creature(); CWorld* world = GetPWorld();
        C3DVector preferred = world->PThingCombatManager_ptr->GetPreferredPos(actor);
        const C3DVector& position = Creature()->GetPos();
        float dx = preferred.X - position.X, dy = preferred.Y - position.Y;
        float reach = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + ReachEpsilon + PreferredPosMargin;   // written e-first: VC7.1 (no /Op) emits these two adds swapped (retail order: 0.3 then 1e-4)
        if (dx * dx + dy * dy < reach * reach) return true;
        const C3DVector& target = ((CThingCreatureBase*)PEnemy()->GetPItem())->GetPos();
        const C3DVector& current = Creature()->GetPos();
        C3DVector delta; delta.X = target.X - current.X; delta.Y = target.Y - current.Y; delta.Z = 0;
        CThingCreatureBase* owner = Creature(); CCombatManager* manager = GetPWorld()->PThingCombatManager_ptr;
        if (!manager->IsPositionOccupied(owner)) {
            float limit = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + EnemyMargin + ReachEpsilon;
            if (delta.X * delta.X + delta.Y * delta.Y < limit * limit) return true;
        }
    }
    return false;
}
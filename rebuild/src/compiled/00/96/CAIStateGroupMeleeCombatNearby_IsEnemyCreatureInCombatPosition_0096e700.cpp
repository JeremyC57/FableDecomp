// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: DELTA_NOTIONAL_ZERO bound to two different float pools (0.3 at 0x1238170, 1e-4 at 0x129ba3c); PreferredMargin/DELTA_NOTIONAL_ZERO swapped)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureInCombatPosition() -- true when the creature already stands within reach
//   of its preferred combat position, or, if that position is not taken, within reach of the enemy.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096E700, 297 bytes.
// Callees: the creature's combatant test 0x00662410 (forwards to the world combat manager's 0x006E92C0; manifest label
//   propagated), CAIStateGroupBase::GetPWorld 0x008FCFC0 (this TU's copy of PBrain->+0x1c->+0x24; 0x008FCFA0 is the other),
//   CCombatManager::GetPreferredPos(CThingCreatureBase*) 0x006EA770 (manifest), CCombatManager position-occupied test
//   0x006E9700 (autoname label only), CThing::GetPos 0x004C73D0 (physics +0x60, position +0x0c),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Constants: four const float globals in .rdata (separate operands: VC would fold literal 0.3f + 1e-4f into one):
//   ReachScale 0.8333333 (0x012925CC), PreferredPosMargin 0.3 (0x01238170), DELTA_NOTIONAL_ZERO 1e-4 (0x0129BA3C),
//   EnemyMargin 1.0 (0x0122DED8); reach = (r + r) * ReachScale + margin + DELTA_NOTIONAL_ZERO. Names are descriptive.
// Layout: engine/CAIStateGroupBase.h (PBrain +0x04), CAIBrain.h (Creature +0x18), CWorld.h (PThingCombatManager_ptr
//   +0x4c); the group's enemy pointer at +0x14 (PDB: unnamed base bytes) and the physics radius at +0x34 are retail
//   offsets. The *_Methods structs are the lane's method carriers for generated data-only headers.
// Name fix (agentNF6 2026-11-10): CCombatManager::GetPreferredPos 0x006EA770 is ?GetPreferredPos@CCombatManager@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z (Ego_r.pdb / FableWin.pdb): const, creature by reference.
#include "fable_compiler.h"
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
#include "engine/C3DVector.h"
extern const float ReachScale;                          // 0x012925CC = 0.8333333
extern const float PreferredPosMargin;                  // 0x01238170 = 0.3
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C = 1e-4
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCombatManager { public:
    C3DVector GetPreferredPos(const CThingCreatureBase& creature) const;                // 0x006EA770
    bool IsPositionOccupied(CThingCreatureBase* creature);                  // 0x006E9700
};
struct CAIStateGroup_MeleeCombatNearby_Methods : CAIStateGroupBase {   // the engine header's flat layout past the base: +0x14 enemy pointer, +0x1c Archer
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    bool          Archer;                                    // +0x1c
    unsigned char _pad_0x1d[0x3];                            // +0x1d
    FABLE_FORCEINLINE CThingCreatureBase* Creature() { return (CThingCreatureBase*)((CAIStateGroupBase*)this)->PBrain->Creature; }
    FABLE_FORCEINLINE CWorld* GetPWorld() { return &GetWorld(); }
    FABLE_FORCEINLINE CBaseIntelligentPointer* PEnemy() { return (CBaseIntelligentPointer*)_pad_0x14; }   // +0x14
    bool IsEnemyCreatureInCombatPosition();
};
bool CAIStateGroup_MeleeCombatNearby_Methods::IsEnemyCreatureInCombatPosition() {
    if (Creature()->IsInMeleeAsAttacker()) {
        CThingCreatureBase* actor = Creature(); CWorld* world = GetPWorld();
        C3DVector preferred = world->PThingCombatManager_ptr->GetPreferredPos(*actor);
        const C3DVector& position = Creature()->GetPos();
        float dx = preferred.X - position.X, dy = preferred.Y - position.Y;
        float reach = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + DELTA_NOTIONAL_ZERO + PreferredPosMargin;   // written e-first: VC7.1 (no /Op) emits these two adds swapped (retail order: 0.3 then 1e-4)
        if (dx * dx + dy * dy < reach * reach) return true;
        const C3DVector& target = ((CThingCreatureBase*)PEnemy()->GetPItem())->GetPos();
        const C3DVector& current = Creature()->GetPos();
        C3DVector delta; delta.X = target.X - current.X; delta.Y = target.Y - current.Y; delta.Z = 0;
        CThingCreatureBase* owner = Creature(); CCombatManager* manager = GetPWorld()->PThingCombatManager_ptr;
        if (!manager->IsPositionOccupied(owner)) {
            float limit = (Creature()->Physics->Radius + Creature()->Physics->Radius) * ReachScale + EnemyMargin + DELTA_NOTIONAL_ZERO;
            if (delta.X * delta.X + delta.Y * delta.Y < limit * limit) return true;
        }
    }
    return false;
}
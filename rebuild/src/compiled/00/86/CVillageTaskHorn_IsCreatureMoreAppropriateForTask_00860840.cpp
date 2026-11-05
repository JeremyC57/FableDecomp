// CVillageTaskHorn::IsCreatureMoreAppropriateForTask(const CThing&) const -- true when the candidate creature is more than 1.25x nearer to the horn than the creature currently assigned (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860840, 247 bytes (256 with padding), `ret 4`.
//   PDB ?IsCreatureMoreAppropriateForTask@CVillageTaskHorn@@QBE_NABVCThing@@@Z (non-virtual).
// Identity: the manifest labels this row "CVillageTaskHorn::IsCreatureMoreAppropriateForTask" (propagated across the
//   byte twins 0x00860650 / 0x00860840); by address it sits in the CVillageTaskHorn block (after Horn AssignTarget 0x00860750 / RemoveThingFromTask 0x008607A0), and the PDB lists the
//   method for Torch, Horn, Patrol, SecuritySweep and CarryCrate.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2), CThing::GetPos 0x004C73D0 (x3). Constant 1.5625f
//   0x01276D70 (a literal).
// Layout (Ego_r.pdb = retail): PHorn +0x08, CreatureAssignedToTask +0x10.
// Shape: the landed CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask (0x00860ED0) with the goal point taken
//   from the PHorn thing: all three positions are by-value copies (the candidate's X/Y kept on the x87 stack), the
//   two deltas named locals, the candidate side of `<` evaluated first.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/C3DVector.h"
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CVillageTaskHorn { public:                         // vtable 0x0125A8D8
    virtual ~CVillageTaskHorn();
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer PHorn;                 // +0x08 CIntelligentPointer<CThing>
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskHorn::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    C3DVector horn = static_cast<const CThing*>(PHorn.GetPItem())->GetPos();
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPos();
    C3DVector position = creature.GetPos();
    C3DVector assignedDelta = horn - assigned;
    C3DVector creatureDelta = horn - position;
    return creatureDelta.GetSquaredMagnitude() * 1.5625f < assignedDelta.GetSquaredMagnitude();
}
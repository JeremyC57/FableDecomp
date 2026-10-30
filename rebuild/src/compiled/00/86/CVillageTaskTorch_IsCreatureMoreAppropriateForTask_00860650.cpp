// CVillageTaskTorch::IsCreatureMoreAppropriateForTask(const CThing&) const -- true when the candidate creature is more than 1.25x nearer to the torch than the creature currently assigned (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860650, 247 bytes (256 with padding), `ret 4`.
//   PDB ?IsCreatureMoreAppropriateForTask@CVillageTaskTorch@@QBE_NABVCThing@@@Z (non-virtual).
// Identity: the manifest labels this row "CVillageTaskHorn::IsCreatureMoreAppropriateForTask" (propagated across the
//   byte twins 0x00860650 / 0x00860840); by address it sits in the CVillageTaskTorch block (after Torch AssignTarget 0x00860560 / RemoveThingFromTask 0x008605B0), and the PDB lists the
//   method for Torch, Horn, Patrol, SecuritySweep and CarryCrate.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2), CThing::GetPos 0x004C73D0 (x3). Constant 1.5625f
//   0x01276D70 (a literal).
// Layout (Ego_r.pdb = retail): Torch +0x08, CreatureAssignedToTask +0x10.
// Shape: the landed CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask (0x00860ED0) with the goal point taken
//   from the Torch thing: all three positions are by-value copies (the candidate's X/Y kept on the x87 stack), the
//   two deltas named locals, the candidate side of `<` evaluated first.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskTorch { public:                         // vtable 0x0125A8AC
    virtual ~CVillageTaskTorch();
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer Torch;                 // +0x08 CIntelligentPointer<CThing>
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskTorch::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    C3DVector torch = static_cast<const CThing*>(Torch.GetPItem())->GetPos();
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPos();
    C3DVector position = creature.GetPos();
    C3DVector assignedDelta = torch - assigned;
    C3DVector creatureDelta = torch - position;
    return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
}
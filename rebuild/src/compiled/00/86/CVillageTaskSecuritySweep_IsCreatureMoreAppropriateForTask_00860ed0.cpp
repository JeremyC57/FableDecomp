// CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask(const CThing& creature) -- true when the candidate creature is more than 1.25x nearer to the sweep point than the creature currently assigned (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860ED0, 205 bytes (padded to 208), `ret 4`.
//   PDB name ?IsCreatureMoreAppropriateForTask@CVillageTaskSecuritySweep@@QBE_NABVCThing@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (CreatureAssignedToTask +0x08), CThing::GetPos 0x004C73D0
//   (x2). Constant 1.5625f 0x01276D70 (a literal: `x * k` loads x first, then `fmul [pool]`).
// Layout (engine/CVillageTaskSecuritySweep.h): CreatureAssignedToTask +0x08, PositionToGoTo +0x10.
// Shape: both positions are by-value copies of the const references (the assigned one fully copied through integer
//   registers, the candidate's X/Y kept on the x87 stack); the deltas are named locals; the candidate side of the
//   `<` is evaluated first (left operand), as in the IsBetterThan twin 0x00860430.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/C3DVector.h"
class CThing : public CBaseObject { public:
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<const CThing>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskSecuritySweep { public:                 // vtable 0x0125A868
    virtual ~CVillageTaskSecuritySweep();          // slot 0
    virtual void v01();
    virtual long GetTaskType() const;              // slot 2 0x008602F0
    virtual bool IsValid() const;                  // slot 3 0x00860300
    virtual bool IsTaskAvailable() const;          // slot 4 0x00860FC0 (+0x10)
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08
    C3DVector PositionToGoTo;                      // +0x10
    float RadiusAroundThePositionToStay;           // +0x1c
    C3DVector PositionToLookTowards;               // +0x20
    float AngleToLookTowards;                      // +0x2c
    bool HasBecomeInvalid;                         // +0x30
    bool IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const;
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPos();
    C3DVector position = creature.GetPos();
    C3DVector assignedDelta = PositionToGoTo - assigned;
    C3DVector creatureDelta = PositionToGoTo - position;
    return creatureDelta.GetSquaredMagnitude() * 1.5625f < assignedDelta.GetSquaredMagnitude();
}
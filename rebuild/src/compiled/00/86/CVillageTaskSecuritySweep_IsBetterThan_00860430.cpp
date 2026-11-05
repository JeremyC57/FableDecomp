// CVillageTaskSecuritySweep::IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) -- with another task to compare against (and this one available) this task wins when its sweep point is nearer to the thing; with no rival the answer is IsTaskAvailable()
// Leaf: IsBetterThan
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860430, 204 bytes (padded to 208), `ret 8`.
//   PDB name ?IsBetterThan@CVillageTaskSecuritySweep@@QBE_NPBV1@ABVCThing@@@Z.
// Callees: virtual IsTaskAvailable (slot 4, +0x10 of vtable 0x0125A868), CThing::GetPos 0x004C73D0.
// Layout (engine/CVillageTaskSecuritySweep.h): CreatureAssignedToTask +0x08, PositionToGoTo +0x10.
// Shape: the thing's position is a by-value copy (X/Y kept on the x87 stack, Z copied through ecx); the two deltas
//   are named locals (this one spilled, the rival's mostly on the stack); `myDist < rivalDist` evaluates left first
//   (`fcompp ; test ah,41h ; jne`).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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
bool CVillageTaskSecuritySweep::IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const
{
    if (other && IsTaskAvailable()) {
        C3DVector position = thing.GetPos();
        C3DVector myDelta = PositionToGoTo - position;
        C3DVector rivalDelta = other->PositionToGoTo - position;
        return myDelta.GetSquaredMagnitude() < rivalDelta.GetSquaredMagnitude();
    }
    return IsTaskAvailable();
}
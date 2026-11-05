// CThingCreatureBase::GetCurrentSpeed() const -- distance the creature moved since the last frame: |PhysicsTC->Position - PhysicsTC->OldPosition|
// Leaf: ?GetCurrentSpeed@CThingCreatureBase@@QBEMXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation); local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661CD0, 46 bytes + 2 int3, thiscall `ret`, float in st(0). Name: Ego_r.pdb
// `?GetCurrentSpeed@CThingCreatureBase@@QBEMXZ` (name set); the landed caller NTCScriptedControl::CActionFollowCreature::
// Update 0x00713EE0 already calls 0x00661CD0 as GetCurrentSpeed (also CTCCreatureNavigation::FrameUpdate 0x006CC88B /
// 0x006CC8AA). No callees (fsqrt inline).
// Label correction: the catalog "Entity_GetDistance" is descriptive.
// Layout: retail CThing+0x60 = PhysicsTC (PDB +0x70: CVectorMap 0x10 and the 4-byte list are smaller in retail);
// CTCPhysicsBase (Ego_r.pdb) Position +0x0C, OldPosition +0x18. Local models (no engine headers for these classes).
// Shape: `(PhysicsTC->Position - PhysicsTC->OldPosition).GetLength()` through the landed inline C3DVector operator- /
// GetLengthSquared (0x007A2F90 model; the temporary on the x87 stack squares Z, Y, X) and `(float)sqrt` -> fsqrt.
// No pragma.
#include <math.h>
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                       // +0x0c
    C3DVector     OldPosition;                    // +0x18
};

#include "engine/CThing.h"
class CThingPhysical : public CThing { public: };
class CThingCreatureBase : public CThingPhysical { public:
    float GetCurrentSpeed() const;
};

float CThingCreatureBase::GetCurrentSpeed() const
{
    return (PhysicsTC->Position - PhysicsTC->OldPosition).GetMagnitude();
}
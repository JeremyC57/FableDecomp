// CTCPhysicsLight::OnCreate() -- CTCPhysicsBase::OnCreate, then AffectedByGravity = (the physics def's mass > 0)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007232E0, 62 bytes, CTCPhysicsLight vtable 0x01265DFC slot 4.
// Callees: CTCPhysicsBase::OnCreate @ 0x006B0780, 0x006AFE00 (returns PhysicsDef->+0x38 = PDB CPhysicsDef::Mass; manifest
//   "CTCPhysicsBase::GetAirResistance" -- AirResistance is +0x34 in the PDB). 0x0129BA3C = 0.0f.
// Layout (PDB CTCPhysicsLight, 0x44; no engine header): CTCPhysicsBase 0x40 (engine/CTCPhysicsBase.h), AffectedByGravity bool:1 at +0x40.
// Shape: a bool:1 assigned a float compare: the compare branches and each arm inserts its own constant through the
//   x ^= (x ^ v) & 1 sequence (`mov al,1` / `xor al,al` not folded).
// Leaf: OnCreate
#include "engine/CTCPhysicsBase.h"

extern float g_fZero;                       // 0x0129BA3C (0.0f)

struct CTCPhysicsBase_Calls {
    void  OnCreate();                       // CTCPhysicsBase::OnCreate 0x006B0780
    float GetMass();                        // 0x006AFE00 -> PhysicsDef->Mass
};

struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;             // +0x40 bit 0
    CTCPhysicsBase_Calls* Base() { return reinterpret_cast<CTCPhysicsBase_Calls*>(this); }
    void OnCreate();
};

void CTCPhysicsLight::OnCreate()
{
    Base()->OnCreate();
    AffectedByGravity = Base()->GetMass() > g_fZero;
}

// CTCPhysicsBase::HasPositiveMass() -- whether the physics def's value at +0x38 (mass) is greater than zero
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B14B0, 28 bytes, CTCPhysicsBase vtable slot 39 (catalogued as vfunc_39). No callees.
// Layout: engine/CTCPhysicsBase.h (PhysicsDef_Object +0x38 = CDefPointer<CPhysicsDef>::Object); CPhysicsDef +0x38 float.
//   0x0129BA3C is the 0.0f pool constant (also compared in CTCPhysicsLight::OnCreate against GetAirResistance()).
// Shape: `fcomp [zero] ; test ah,41h ; jne` = `value > zero`; a long 0/1 result (`mov eax,1` / `xor eax,eax`).
// Leaf: HasPositiveMass
#include "engine/CTCPhysicsBase.h"

class CPhysicsDef { public:
    unsigned char _pad00[0x38];
    float         Mass;                     // +0x38
};

extern float g_fZero;                       // 0x0129BA3C (0.0f)

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    const CPhysicsDef* GetDef() const { return reinterpret_cast<const CPhysicsDef*>(PhysicsDef_Object); }
    long HasPositiveMass() const;
};

long CTCPhysicsBase_Methods::HasPositiveMass() const
{
    return GetDef()->Mass > g_fZero;
}
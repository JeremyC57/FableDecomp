// CTCPhysicsBase::GetMass() -- the physics def's mass
// Leaf: ?GetMass@CTCPhysicsBase@@
// Renamed (fistp pass, 2026-10-24) from CTCPhysicsBase::GetAirResistance @ 00/6a/CTCPhysicsBase_GetAirResistance_006afe00.cpp: reads PhysicsDef (+0x38) -> CPhysicsDef::Mass (+0x38, Ego_r.pdb: Friction +0x2C, Elasticity +0x30, AirResistance +0x34, Mass +0x38); callers CTCPhysicsLight::OnCreate 0x007232EA, CTCPhysicsStandard 0x007235F4 / OnCreate 0x007240C2
// (was: CTCPhysicsBase::GetMass() -- returns the float at Mass (+0x38) of the PhysicsDef (+0x38))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AFE00, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x38] ; fld dword ptr [eax + 0x38] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CPhysicsDef {
    unsigned char _pad[0x38];
    float Mass;                         // +0x38
};

struct CTCPhysicsBase {
    unsigned char _pad[0x38];
    CPhysicsDef* PhysicsDef;                     // +0x38
    float GetMass();
};

float CTCPhysicsBase::GetMass()
{
    return PhysicsDef->Mass;
}

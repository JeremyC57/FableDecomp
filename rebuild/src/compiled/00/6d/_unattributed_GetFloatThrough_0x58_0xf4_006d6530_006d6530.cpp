// _unattributed::GetFloatThrough_0x58_0xf4_006d6530() -- returns the float at +0xf4 of the object pointed to by this+0x58; owner class unknown
// Leaf: ?GetFloatThrough_0x58_0xf4_006d6530@_unattributed@@
// Renamed (fistp pass, 2026-10-24) from CTCPhysicsBase::GetAirResistance @ 00/6d/CTCPhysicsBase_GetAirResistance_006d6530.cpp: not in any vtable; this+0x58 is not CTCPhysicsBase::PhysicsDef (+0x38), so the propagated GetAirResistance label is wrong; caller(s): CThingCreatureBase::GetAnimationSpeedMultiplier (0x00665709); descriptive name until the owning TC is identified
// (was: _unattributed::GetFloatThrough_0x58_0xf4_006d6530() -- returns the float at +0xf4 of the object at +0x58)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6530, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x58] ; fld dword ptr [eax + 0xf4] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0xf4];
    float m_value;                         // +0xf4
};

struct _unattributed {
    unsigned char _pad[0x58];
    CTarget* m_object;                     // +0x58
    float GetFloatThrough_0x58_0xf4_006d6530();
};

float _unattributed::GetFloatThrough_0x58_0xf4_006d6530()
{
    return m_object->m_value;
}

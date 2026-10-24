// _unattributed::GetFloatThrough_0x08_0x2c_00810f30() -- returns the float at +0x2c of the object pointed to by this+0x08; owner class unknown
// Leaf: ?GetFloatThrough_0x08_0x2c_00810f30@_unattributed@@
// Renamed (fistp pass, 2026-10-24) from CTCPhysicsBase::GetAirResistance @ 00/81/CTCPhysicsBase_GetAirResistance_00810f30.cpp: not in any vtable; this+0x08 is not CTCPhysicsBase::PhysicsDef (+0x38), so the propagated GetAirResistance label is wrong; caller(s): CGameScriptThing::MsgIsHitByWithProjectileWeapon (0x008D16A4); descriptive name until the owning TC is identified
// (was: _unattributed::GetFloatThrough_0x08_0x2c_00810f30() -- returns the float at +0x2c of the object at +0x8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00810F30, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 8] ; fld dword ptr [eax + 0x2c] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x2c];
    float m_value;                         // +0x2c
};

struct _unattributed {
    unsigned char _pad[0x8];
    CTarget* m_object;                     // +0x8
    float GetFloatThrough_0x08_0x2c_00810f30();
};

float _unattributed::GetFloatThrough_0x08_0x2c_00810f30()
{
    return m_object->m_value;
}

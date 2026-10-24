// _unattributed::GetFloatThrough_0x20_0x12c_0079c740() -- returns the float at +0x12c of the object pointed to by this+0x20; owner class unknown
// Leaf: ?GetFloatThrough_0x20_0x12c_0079c740@_unattributed@@
// Renamed (fistp pass, 2026-10-24) from CTCPhysicsBase::GetAirResistance @ 00/79/CTCPhysicsBase_GetAirResistance_0079c740.cpp: not in any vtable; this+0x20 is not CTCPhysicsBase::PhysicsDef (+0x38), so the propagated GetAirResistance label is wrong; caller(s): unlabelled functions at 0x006A036C / 0x008CDEB7; descriptive name until the owning TC is identified
// (was: _unattributed::GetFloatThrough_0x20_0x12c_0079c740() -- returns the float at +0x12c of the object at +0x20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C740, oracle length 10 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x20] ; fld dword ptr [eax + 0x12c] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x12c];
    float m_value;                         // +0x12c
};

struct _unattributed {
    unsigned char _pad[0x20];
    CTarget* m_object;                     // +0x20
    float GetFloatThrough_0x20_0x12c_0079c740();
};

float _unattributed::GetFloatThrough_0x20_0x12c_0079c740()
{
    return m_object->m_value;
}

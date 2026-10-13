// CTCPhysicsBase::GetAirResistance() -- returns the float at +0x44 of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751B70, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x14] ; fld dword ptr [eax + 0x44] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x44];
    float m_value;                         // +0x44
};

struct CTCPhysicsBase {
    unsigned char _pad[0x14];
    CTarget* m_object;                     // +0x14
    float GetAirResistance();
};

float CTCPhysicsBase::GetAirResistance()
{
    return m_object->m_value;
}
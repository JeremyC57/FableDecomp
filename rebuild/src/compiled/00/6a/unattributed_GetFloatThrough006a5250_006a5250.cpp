// CClass_006a5250::GetFloatThrough_006a5250() -- returns the float at +0x54 of the object at +0x70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A5250, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x70] ; fld dword ptr [eax + 0x54] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x54];
    float m_value;                         // +0x54
};

struct CClass_006a5250 {
    unsigned char _pad[0x70];
    CTarget* m_object;                     // +0x70
    float GetFloatThrough_006a5250();
};

float CClass_006a5250::GetFloatThrough_006a5250()
{
    return m_object->m_value;
}
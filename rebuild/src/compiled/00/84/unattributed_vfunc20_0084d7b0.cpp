// CClass_0084d7b0::vfunc_20() -- returns the float at +0x4c of the object at +0x44
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084D7B0, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x44] ; fld dword ptr [eax + 0x4c] ; ret 
// Tiny-body shape `GetFloatThrough` synthesised from the instruction pattern (the class comes from the
// vtable-slot/manifest label when it is a plain class name, else the method is landed under a
// descriptive placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x4c];
    float m_value;                         // +0x4c
};

struct CClass_0084d7b0 {
    unsigned char _pad[0x44];
    CTarget* m_object;                     // +0x44
    float vfunc_20();
};

float CClass_0084d7b0::vfunc_20()
{
    return m_object->m_value;
}
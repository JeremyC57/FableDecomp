// CClass_0084d7d0::vfunc_8() -- returns the byte at +0x61 of the object pointed to by +0x44
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084D7D0, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x44] ; mov al,[eax+0x61] ; ret
// Virtual getter through one pointer member: loads the object at +0x44 and returns its byte at
// +0x61 in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x61];
    unsigned char m_value;                 // +0x61
};

struct CClass_0084d7d0 {
    virtual void v00();
    virtual unsigned char vfunc_8();
    unsigned char _pad[0x40];
    CTarget* m_target;                     // +0x44
};

unsigned char CClass_0084d7d0::vfunc_8()
{
    return m_target->m_value;
}
// CClass_00810ef0::vfunc_5() -- returns the byte at +0x1d of the object pointed to by +0x8
// Leaf: vfunc_5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00810EF0, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x8] ; mov al,[eax+0x1d] ; ret
// Virtual getter through one pointer member: loads the object at +0x8 and returns its byte at
// +0x1d in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x1d];
    unsigned char m_value;                 // +0x1d
};

struct CClass_00810ef0 {
    virtual void v00();
    virtual unsigned char vfunc_5();
    unsigned char _pad[0x4];
    CTarget* m_target;                     // +0x8
};

unsigned char CClass_00810ef0::vfunc_5()
{
    return m_target->m_value;
}
// CTCPhysicsBase::vfunc_83() -- returns the byte at +0x40 of the object pointed to by +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AFE50, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x38] ; mov al,[eax+0x40] ; ret
// Virtual getter through one pointer member: loads the object at +0x38 and returns its byte at
// +0x40 in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x40];
    unsigned char m_value;                 // +0x40
};

struct CTCPhysicsBase {
    virtual void v00();
    virtual unsigned char vfunc_83();
    unsigned char _pad[0x34];
    CTarget* m_target;                     // +0x38
};

unsigned char CTCPhysicsBase::vfunc_83()
{
    return m_target->m_value;
}
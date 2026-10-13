// CTCShotDefinitionBase::CanBeHitByMeleeStrike() -- returns the byte at +0x1a of the object pointed to by +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00810F00, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0x8] ; mov al,[eax+0x1a] ; ret
// Virtual getter through one pointer member: loads the object at +0x8 and returns its byte at
// +0x1a in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x1a];
    unsigned char m_value;                 // +0x1a
};

struct CTCShotDefinitionBase {
    virtual void v00();
    virtual unsigned char CanBeHitByMeleeStrike();
    unsigned char _pad[0x4];
    CTarget* m_target;                     // +0x8
};

unsigned char CTCShotDefinitionBase::CanBeHitByMeleeStrike()
{
    return m_target->m_value;
}

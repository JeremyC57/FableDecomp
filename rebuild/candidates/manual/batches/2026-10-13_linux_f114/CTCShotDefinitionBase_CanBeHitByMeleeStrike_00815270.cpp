// CTCShotDefinitionBase::CanBeHitByMeleeStrike() -- returns the byte at +0x6c of the object pointed to by +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815270, oracle length 7 bytes.
// Grade: candidate -- still SRC_FAIL in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  mov eax,[ecx+0x4] ; mov al,[eax+0x6c] ; ret
// Virtual getter through one pointer member: loads the object at +0x4 and returns its byte at
// +0x6c in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x6c];
    unsigned char m_value;                 // +0x6c
};

struct CTCShotDefinitionBase {
    virtual void v00();
    virtual unsigned char CanBeHitByMeleeStrike();
    unsigned char _pad[0x0];
    CTarget* m_target;                     // +0x4
};

unsigned char CTCShotDefinitionBase::CanBeHitByMeleeStrike()
{
    return m_target->m_value;
}

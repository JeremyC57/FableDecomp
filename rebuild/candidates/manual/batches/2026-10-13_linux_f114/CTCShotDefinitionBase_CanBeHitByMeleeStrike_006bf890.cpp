// CTCShotDefinitionBase::CanBeHitByMeleeStrike() -- returns the byte at +0x26 of the object pointed to by +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF890, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+0xc] ; mov al,[eax+0x26] ; ret
// Virtual getter through one pointer member: loads the object at +0xc and returns its byte at
// +0x26 in al. Classes and member names are descriptive (vtable-slot label for the class).
// One of 11 bodies of this shape.
struct CTarget {
    unsigned char _pad[0x26];
    unsigned char m_value;                 // +0x26
};

struct CTCShotDefinitionBase {
    virtual void v00();
    virtual unsigned char CanBeHitByMeleeStrike();
    unsigned char _pad[0x8];
    CTarget* m_target;                     // +0xc
};

unsigned char CTCShotDefinitionBase::CanBeHitByMeleeStrike()
{
    return m_target->m_value;
}

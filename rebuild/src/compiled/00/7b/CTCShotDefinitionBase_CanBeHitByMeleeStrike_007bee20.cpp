// CTCShotDefinitionBase::CanBeHitByMeleeStrike() -- returns the byte at +0x91 of the object at +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BEE20, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x38] ; mov al, byte ptr [eax + 0x91] ; ret 
// Tiny-body shape `GetByteThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x91];
    unsigned char m_value;                 // +0x91
};

struct CTCShotDefinitionBase {
    unsigned char _pad[0x38];
    CTarget* m_object;                     // +0x38
    unsigned char CanBeHitByMeleeStrike();
};

unsigned char CTCShotDefinitionBase::CanBeHitByMeleeStrike()
{
    return m_object->m_value;
}
// CTCShotDefinitionBase::CanBeHitByMeleeStrike() -- returns the byte at +0x6e of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815230, oracle length 7 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov al, byte ptr [eax + 0x6e] ; ret 
// Tiny-body shape `GetByteThrough` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x6e];
    unsigned char m_value;                 // +0x6e
};

struct CTCShotDefinitionBase {
    unsigned char _pad[0x4];
    CTarget* m_object;                     // +0x4
    unsigned char CanBeHitByMeleeStrike();
};

unsigned char CTCShotDefinitionBase::CanBeHitByMeleeStrike()
{
    return m_object->m_value;
}
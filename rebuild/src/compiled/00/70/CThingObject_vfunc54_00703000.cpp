// CThingObject::vfunc_54() -- the 1-bit bitfield at bit 0 of the flag byte at +0x98 of the object at +0xd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703000, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xd0] ; mov al, byte ptr [eax + 0x98] ; and al, 1 ; ret 
// Tiny-body shape `GetTargetBits` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x98];
    unsigned char m_field : 1;             // +0x98, bits 0..0
};

struct CThingObject {
    unsigned char _pad[0xd0];
    CTarget* m_pTarget;                    // +0xd0
    bool vfunc_54();
};

bool CThingObject::vfunc_54()
{
    return m_pTarget->m_field;
}
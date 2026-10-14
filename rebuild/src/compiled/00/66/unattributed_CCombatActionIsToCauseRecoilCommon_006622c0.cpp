// CClass_006622c0::CCombatAction_IsToCauseRecoilCommon() -- the 1-bit bitfield at bit 0 of the flag byte at +0x149 of the object at +0xe0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006622C0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0xe0] ; mov al, byte ptr [eax + 0x149] ; and al, 1 ; ret 
// Tiny-body shape `GetTargetBits` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad[0x149];
    unsigned char m_field : 1;             // +0x149, bits 0..0
};

struct CClass_006622c0 {
    unsigned char _pad[0xe0];
    CTarget* m_pTarget;                    // +0xe0
    bool CCombatAction_IsToCauseRecoilCommon();
};

bool CClass_006622c0::CCombatAction_IsToCauseRecoilCommon()
{
    return m_pTarget->m_field;
}
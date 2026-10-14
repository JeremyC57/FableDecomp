// CCombatActionBase::GetAnimationIndex() -- the first dword of the object at +0x74
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008565B0, oracle length 6 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x74] ; mov eax, dword ptr [eax] ; ret 
// Tiny-body shape `GetTargetFirst` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    void* m_pFirst;                        // +0
};

struct CCombatActionBase {
    unsigned char _pad[0x74];
    CTarget* m_pTarget;                    // +0x74
    void* GetAnimationIndex();
};

void* CCombatActionBase::GetAnimationIndex()
{
    return m_pTarget->m_pFirst;
}
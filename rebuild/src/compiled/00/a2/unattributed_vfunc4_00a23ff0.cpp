// CClass_00a23ff0::vfunc_4() -- the target at +0x1c's predicate @ 0x009D54E0 (tail-called), or 1 when there is no target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23FF0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 0x1c] ; test ecx, ecx ; je 0xa23ffc ; jmp 0x9d54e0 ; mov al, 1 ; ret 
// Tiny-body shape `QueryTarget` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    bool Query();                          // 0x009D54E0
};

struct CClass_00a23ff0 {
    unsigned char _pad[0x1c];
    CTarget* m_pTarget;                    // +0x1c
    bool vfunc_4();
};

bool CClass_00a23ff0::vfunc_4()
{
    if (m_pTarget)
        return m_pTarget->Query();
    return true;
}
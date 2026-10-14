// CClass_00a35f70::GetScaledTargetValue_00a35f70() -- the float at +0x34 of the object at +0x4 times the scale at +0x74
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A35F70, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; fld dword ptr [eax + 0x34] ; fmul dword ptr [ecx + 0x74] ; ret 
// Tiny-body shape `GetScaledTargetValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTarget {
    unsigned char _pad1[0x34];
    float m_value;                         // +0x34
};

struct CClass_00a35f70 {
    unsigned char _pad2[0x4];
    CTarget* m_pTarget;                     // +0x4
    unsigned char _pad3[0x6c];
    float m_scale;                          // +0x74
    float GetScaledTargetValue_00a35f70();
};

float CClass_00a35f70::GetScaledTargetValue_00a35f70()
{
    return m_scale * m_pTarget->m_value;
}
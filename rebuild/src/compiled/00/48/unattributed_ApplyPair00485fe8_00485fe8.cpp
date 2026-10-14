#pragma optimize("s",on)
// CClass_00485fe8::ApplyPair_00485fe8() -- calls the member method @ 0x0043336A with the members at +0 and +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485FE8, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push dword ptr [ecx + 4] ; push dword ptr [ecx] ; call 0x43336a ; ret 
// Tiny-body shape `ApplyPair` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00485fe8 {
    void* m_first;                         // +0
    void* m_second;                        // +0x4
    void Apply(void* first, void* second); // 0x0043336A
    void ApplyPair_00485fe8();
};

void CClass_00485fe8::ApplyPair_00485fe8()
{
    Apply(m_first, m_second);
}
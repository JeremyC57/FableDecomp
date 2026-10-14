#pragma optimize("s",on)
// CPersistContext_TransferableVector::vfunc_0() -- element count of the {first, last} range object at +0x4 (20-byte elements, signed division)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0923, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [ecx + 4] ; mov eax, dword ptr [ecx + 4] ; sub eax, dword ptr [ecx] ; push 0x14 ; cdq  ; pop ecx ; idiv ecx ; ret 
// Tiny-body shape `GetCountThroughRange` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SRange {
    long m_first;                          // byte positions: the elements are 20 bytes
    long m_last;
};

struct CPersistContext_TransferableVector {
    unsigned char _pad[0x4];
    SRange* m_pRange;                      // +0x4
    long vfunc_0();
};

long CPersistContext_TransferableVector::vfunc_0()
{
    return (m_pRange->m_last - m_pRange->m_first) / 20;
}
// SRecord_00cb12e0::SRecord_00cb12e0(long) -- zero a six-word record whose fourth word starts at 10; the argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB12E0, length 28 bytes. No callees.
// Leaf: ??0SRecord_00cb12e0
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated (callers 0x00CAA688, 0x00CABE47,
// 0x00CAEEB8, 0x00CAF532, 0x00CAF540 in the file-archive module). `ret 4` with the stack slot never read = one ignored parameter.
// Shape: a plain initialiser list in member order sharing the ecx zero, `mov dword [eax+0Ch],0Ah` as an immediate.
struct SRecord_00cb12e0 {
    long m_a;          // +0x00
    long m_b;          // +0x04
    long m_c;          // +0x08
    long m_capacity;   // +0x0c (starts at 10)
    long m_e;          // +0x10
    long m_f;          // +0x14
    SRecord_00cb12e0(long unused);
};

SRecord_00cb12e0::SRecord_00cb12e0(long) : m_a(0), m_b(0), m_c(0), m_capacity(10), m_e(0), m_f(0)
{
}

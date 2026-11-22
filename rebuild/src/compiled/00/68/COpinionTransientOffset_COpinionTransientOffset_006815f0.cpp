// SRecord_006815f0::SRecord_006815f0() -- zero a six-word record, the word at +0x0c last
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006815F0, length 22 bytes (+ int3 padding). No callees.
// Leaf: ??0SRecord_006815f0
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated; the one caller (0x004793D5) is a
// CPersistTraits<std::list<COpinionTransientOffset...>>::TransferBinaryIn, which builds this record as a list element value.
// Shape: `mov eax,ecx ; xor ecx,ecx`, stores +0,+4,+8,+0x10,+0x14 then +0x0c: five initialisers in member order and the
// +0x0c word assigned in the body (a body assignment is scheduled after the init-list stores).
struct SRecord_006815f0 {
    long m_a;      // +0x00
    long m_b;      // +0x04
    long m_c;      // +0x08
    long m_d;      // +0x0c
    long m_e;      // +0x10
    long m_f;      // +0x14
    SRecord_006815f0();
};

SRecord_006815f0::SRecord_006815f0() : m_a(0), m_b(0), m_c(0), m_e(0), m_f(0)
{
    m_d = 0;
}
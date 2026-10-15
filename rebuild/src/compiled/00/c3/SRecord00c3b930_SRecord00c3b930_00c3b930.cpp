// SRecord_00c3b930::SRecord_00c3b930() -- zero a 0x20-byte record, its +0x04 word taken from a global default and two shorts at +0x10/+0x12
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B930, length 39 bytes (+ int3 padding). Reads the global @ 0x0139C900.
// Leaf: ??0SRecord_00c3b930
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated; the caller (0x00C2F860) is inside
// CMessageEvent_ProcessLoadingFrame_Vsync. Shape: plain initialiser list in member order; the two `mov word ptr` stores
// of cx mark 16-bit members.
extern void* g_default_00c3b930;   // 0x0139C900

struct SRecord_00c3b930 {
    long  m_a;         // +0x00
    void* m_default;   // +0x04
    long  m_c;         // +0x08
    long  m_d;         // +0x0c
    short m_e;         // +0x10
    short m_f;         // +0x12
    long  m_g;         // +0x14
    long  m_h;         // +0x18
    long  m_i;         // +0x1c
    SRecord_00c3b930();
};

SRecord_00c3b930::SRecord_00c3b930()
    : m_a(0), m_default(g_default_00c3b930), m_c(0), m_d(0), m_e(0), m_f(0), m_g(0), m_h(0), m_i(0)
{
}
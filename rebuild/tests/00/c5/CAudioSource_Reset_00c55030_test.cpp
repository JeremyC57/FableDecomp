// Behaviour test for 00c55030: CAudioSource::Reset -- rewinds the stream, resets both ramps, clears +0x22c / +0x238
#include <cstdio>
#include <cstring>
static char g_seq[8] = ""; static void* g_reset[2]; static int g_n = 0;
struct CDecodedStream { virtual void v00() { } virtual void Rewind() { std::strcat(g_seq, "R"); } };
struct SRampedVector { float m_current[3], m_rest[3], m_rate[3]; void Reset() { std::strcat(g_seq, "B"); g_reset[g_n++] = this; } };
struct CAudioSource { void* __vftable; unsigned char _pad4[0xd8]; SRampedVector m_rampA; SRampedVector m_rampB; unsigned char _pad124[0xfc]; CDecodedStream* m_pStream; unsigned char _pad224[8]; long m_22c; unsigned char _pad230[8]; long m_238;
    void Reset() { m_pStream->Rewind(); m_rampA.Reset(); m_rampB.Reset(); m_22c = 0; m_238 = 0; } };
int main()
{
    CAudioSource o; std::memset(&o, 0x11, sizeof o); CDecodedStream s; o.m_pStream = &s; o.Reset();
    int ok = !std::strcmp(g_seq, "RBB") && g_reset[0] == &o.m_rampA && g_reset[1] == &o.m_rampB && o.m_22c == 0 && o.m_238 == 0;
    std::printf(ok ? "00c55030_TEST PASS\n" : "00c55030_TEST FAIL\n");
    return ok ? 0 : 1;
}

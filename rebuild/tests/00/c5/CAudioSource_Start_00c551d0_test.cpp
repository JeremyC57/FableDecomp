#include <cstdio>
static int g_rewinds = 0, g_vecResets = 0, g_addRefs = 0;
struct SRampedVector { float m_current[3]; void Reset() { ++g_vecResets; m_current[0] = m_current[1] = m_current[2] = 0.0f; } };
struct CDecodedStream { void Rewind() { ++g_rewinds; } };
struct CSoundDesc { unsigned int m_flags; void AddRef() { ++g_addRefs; } };
struct CPlayParams { int m_loopStart; int m_loopCount; };
struct CAudioSource {
    SRampedVector m_rampA, m_rampB; float m_gainLeft, m_gainRight; CDecodedStream* m_pStream; int m_22c, m_238;
    unsigned int m_configured;
    unsigned int Configure(CSoundDesc* desc, CPlayParams* params) { (void)desc; (void)params; return m_configured; }
    void Reset()
    {
        m_pStream->Rewind();
        m_rampA.Reset();
        m_rampB.Reset();
        m_22c = 0;
        m_238 = 0;
    }
    unsigned int Start(CSoundDesc* desc, CPlayParams* params)
    {
        desc->AddRef();
        unsigned int caps = Configure(desc, params) | 0x1BF8000;
        if (!(desc->m_flags & 0x800000)) {
            m_gainLeft = 1.0f;
            m_gainRight = 1.0f;
            caps &= ~0x800000u;
        }
        if (params->m_loopStart)
            caps |= 0x80;
        else if (params->m_loopCount)
            caps |= 0x100;
        Reset();
        return caps;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c551d0_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CDecodedStream stream; CAudioSource v; v.m_pStream = &stream; v.m_22c = 3; v.m_238 = 4;
    v.m_gainLeft = 0.5f; v.m_gainRight = 0.25f; v.m_configured = 0x00800003; v.m_rampA.m_current[0] = 2.0f;
    CSoundDesc keep; keep.m_flags = 0x800000; CPlayParams p; p.m_loopStart = 1; p.m_loopCount = 9;
    CHECK(v.Start(&keep, &p) == (0x00800003u | 0x1BF8000u | 0x80u));
    CHECK(g_addRefs == 1 && g_rewinds == 1 && g_vecResets == 2 && v.m_22c == 0 && v.m_238 == 0 && v.m_rampA.m_current[0] == 0.0f);
    CHECK(v.m_gainLeft == 0.5f && v.m_gainRight == 0.25f);
    CSoundDesc plain; plain.m_flags = 0x10; p.m_loopStart = 0; p.m_loopCount = 2;
    CHECK(v.Start(&plain, &p) == ((0x00800003u | 0x1BF8000u | 0x100u) & ~0x800000u));
    CHECK(v.m_gainLeft > 0.999f && v.m_gainLeft < 1.001f && v.m_gainRight > 0.999f && v.m_gainRight < 1.001f);
    p.m_loopCount = 0;
    CHECK(v.Start(&plain, &p) == ((0x00800003u | 0x1BF8000u) & ~0x800000u));
    CHECK(g_addRefs == 3 && g_rewinds == 3 && g_vecResets == 6);
    std::printf("00c551d0_TEST PASS\n");
    return 0;
}
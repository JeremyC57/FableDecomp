#include <cstdio>
static int g_primes = 0;
struct CStreamHandle { int primed; void Prime() { ++primed; ++g_primes; } };
static CStreamHandle g_stream;
struct CAudioSource {
    int m_210, m_214; float m_gainLeft, m_gainRight; CStreamHandle* m_pStream;
    unsigned short m_volume, m_targetVolume; bool m_ramping;
    CStreamHandle* CreateStream(void* source) { return source ? &g_stream : 0; }
    bool Open(void* source)
    {
        CStreamHandle* stream = CreateStream(source);
        m_pStream = stream;
        if (!stream)
            return false;
        stream->Prime();
        m_210 = 0;
        m_214 = 0;
        m_gainLeft = 1.0f;
        m_gainRight = 1.0f;
        m_ramping = false;
        m_volume = 0;
        m_targetVolume = 0;
        return true;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c55680_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CAudioSource v; v.m_210 = 5; v.m_214 = 6; v.m_gainLeft = 0.25f; v.m_gainRight = 0.5f; v.m_pStream = &g_stream;
    v.m_volume = 7; v.m_targetVolume = 8; v.m_ramping = true;
    CHECK(!v.Open(0));
    CHECK(v.m_pStream == 0 && g_primes == 0 && v.m_210 == 5 && v.m_ramping && v.m_volume == 7);
    int src = 1;
    CHECK(v.Open(&src));
    CHECK(v.m_pStream == &g_stream && g_primes == 1 && v.m_210 == 0 && v.m_214 == 0);
    CHECK(v.m_gainLeft > 0.999f && v.m_gainLeft < 1.001f && v.m_gainRight > 0.999f && v.m_gainRight < 1.001f);
    CHECK(!v.m_ramping && v.m_volume == 0 && v.m_targetVolume == 0);
    std::printf("00c55680_TEST PASS\n");
    return 0;
}
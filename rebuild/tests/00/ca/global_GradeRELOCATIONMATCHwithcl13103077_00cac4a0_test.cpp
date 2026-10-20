// Behaviour test for CEngineScreenEffectRenderer::OnRenderEffect @ 0x00CAC4A0: the media type is copied into a
// CMediaType local, applied through SetMediaType; a failing HRESULT is returned at once (slot 22 not called), else the
// remaining arguments go to virtual slot 22 and its failure is returned, otherwise S_OK (even for S_FALSE). The local
// is destroyed exactly once on every path.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct AM_MEDIA_TYPE {                                 // 0x48 bytes
    unsigned char _body[0x48];
};
static int g_ctors = 0, g_dtors = 0;
static long* g_lastPhr = (long*)1;
static const void* g_liveLocal = 0;
struct CMediaType : AM_MEDIA_TYPE {
    CMediaType(const AM_MEDIA_TYPE& rt, long* phr = 0)    // 0x00CB1D60 (CopyMediaType)
    { std::memcpy(_body, rt._body, sizeof(_body)); ++g_ctors; g_lastPhr = phr; g_liveLocal = this; }
    ~CMediaType() { ++g_dtors; if (g_liveLocal == this) g_liveLocal = 0; }   // 0x00CB1CC0
};

struct CMediaEffectSink {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {}
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {}
    virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {}
    virtual long RenderEffect(long, long, long, long, double) { g_fail = 1; return -1; }   // slot 22 (+0x58)
};

static long g_setResult = 0;
static int g_setCalls = 0;
static bool g_setSawCopy = false;
static const AM_MEDIA_TYPE* g_source = 0;
struct CEngineScreenEffectRenderer : CMediaEffectSink {
    long SetMediaType(CMediaType* type)                // 0x00CAB990
    {
        ++g_setCalls;
        g_setSawCopy = type == g_liveLocal && (const void*)type != (const void*)g_source
                    && std::memcmp(type->_body, g_source->_body, sizeof(type->_body)) == 0;
        return g_setResult;
    }
    long OnRenderEffect(const AM_MEDIA_TYPE* type, long a, long b, long c, long d, double time);
};

long CEngineScreenEffectRenderer::OnRenderEffect(const AM_MEDIA_TYPE* type, long a, long b, long c, long d, double time)
{
    CMediaType mediaType(*type, 0);
    long hr = SetMediaType(&mediaType);
    if (hr < 0)
        return hr;
    hr = RenderEffect(a, b, c, d, time);
    if (hr < 0)
        return hr;
    return 0;
}

struct TestRenderer : CEngineScreenEffectRenderer {
    int calls; long a, b, c, d; double t; long result; bool localAlive;
    virtual long RenderEffect(long a_, long b_, long c_, long d_, double t_)
    { ++calls; a = a_; b = b_; c = c_; d = d_; t = t_; localAlive = g_liveLocal != 0; return result; }
};

static void reset(TestRenderer& r) { g_ctors = g_dtors = g_setCalls = 0; g_setSawCopy = false; g_lastPhr = (long*)1; r.calls = 0; r.a = r.b = r.c = r.d = 0; r.t = 0; r.localAlive = false; }

int main()
{
    AM_MEDIA_TYPE mt; for (int i = 0; i < 0x48; ++i) mt._body[i] = (unsigned char)(0x30 + i);
    g_source = &mt;
    TestRenderer r;

    // 1. setter fails: its HRESULT returned, slot 22 untouched, local destroyed.
    reset(r); g_setResult = (long)0x80004005; r.result = 0;
    CHECK(r.OnRenderEffect(&mt, 1, 2, 3, 4, 5.0) == (long)0x80004005);
    CHECK(g_ctors == 1 && g_dtors == 1 && g_lastPhr == 0 && g_setCalls == 1 && g_setSawCopy && r.calls == 0);

    // 2. setter succeeds (S_FALSE counts as success), slot 22 fails: its HRESULT returned with the arguments forwarded.
    reset(r); g_setResult = 1; r.result = (long)0x8007000E;
    CHECK(r.OnRenderEffect(&mt, 11, -22, 33, 44, 2.5) == (long)0x8007000E);
    CHECK(r.calls == 1 && r.a == 11 && r.b == -22 && r.c == 33 && r.d == 44 && r.t == 2.5 && r.localAlive);
    CHECK(g_ctors == 1 && g_dtors == 1 && g_setSawCopy);

    // 3. both succeed, slot 22 returns S_FALSE: the result is S_OK.
    reset(r); g_setResult = 0; r.result = 1;
    CHECK(r.OnRenderEffect(&mt, 7, 8, 9, 10, -1.25) == 0);
    CHECK(r.calls == 1 && r.a == 7 && r.d == 10 && r.t == -1.25 && g_ctors == 1 && g_dtors == 1);

    if (g_fail) { std::printf("00cac4a0_TEST FAIL\n"); return 1; }
    std::printf("00cac4a0_TEST PASS\n");
    return 0;
}
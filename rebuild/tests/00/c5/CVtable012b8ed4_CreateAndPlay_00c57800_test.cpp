// Behaviour test for 0x00C57800 CVtable_012b8ed4::CreateAndPlay: channel count -> AL format / block size, source creation,
// one buffer name per 128 bytes, and every failure exit (unknown layout, unknown enum, AL errors, allocation failure).
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_failMalloc = 0, g_freed = 0;
static void* CodaMalloc(unsigned int n) { return g_failMalloc ? 0 : std::malloc(n); }
static void CodaFree(void* p) { ++g_freed; std::free(p); }

struct CAudioContext {
    int errorAfterSources, errorAfterBuffers, enumMissing, phase, loopingParam, loopingValue, genCount;
    long GetEnumValue(const char* name)
    {
        if (enumMissing) return 0;
        if (!std::strcmp(name, "AL_FORMAT_QUAD16")) return 0x1205;
        if (!std::strcmp(name, "AL_FORMAT_51CHN16")) return 0x120b;
        if (!std::strcmp(name, "AL_FORMAT_71CHN16")) return 0x1211;
        return -1;
    }
    long GetError()
    {
        long e = 0;
        if (phase == 1) e = errorAfterSources;
        if (phase == 2) e = errorAfterBuffers;
        return e;
    }
    void GenSources(int count, unsigned int* sources) { *sources = 77; phase = 1; (void)count; }
    void Sourcei(unsigned int source, int param, int value) { loopingParam = param; loopingValue = value; (void)source; }
    void GenBuffers(int count, unsigned int* buffers) { for (int i = 0; i < count; ++i) buffers[i] = 100 + i; genCount = count; phase = 2; }
};
static CAudioContext g_AudioContext;

class CVtable_012b8ed4 {
public:
    bool CreateAndPlay(unsigned int channels, unsigned int bytes);
    unsigned int*  m_pBuffers;
    unsigned int   m_source;
    long           m_format;
    unsigned int   m_blockBytes;
    bool           m_hasSource;
    bool           m_created;
    unsigned int   m_bufferCount;
    unsigned int   m_queuedCount;
    unsigned int   m_620;
    unsigned int   m_624;
};

bool CVtable_012b8ed4::CreateAndPlay(unsigned int channels, unsigned int bytes)
{
    switch (channels) {
    case 1:
        m_format = 0x1101;                          // AL_FORMAT_MONO16
        m_blockBytes = 0x100;
        break;
    case 2:
        m_format = 0x1103;                          // AL_FORMAT_STEREO16
        m_blockBytes = 0x200;
        break;
    case 4:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_QUAD16");
        m_blockBytes = 0x400;
        break;
    case 6:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_51CHN16");
        m_blockBytes = 0x600;
        break;
    case 8:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_71CHN16");
        m_blockBytes = 0x800;
        break;
    default:
        return false;
    }
    if (m_format == 0)
        return false;
    g_AudioContext.GetError();
    g_AudioContext.GenSources(1, &m_source);
    if (g_AudioContext.GetError() != 0)
        return false;
    g_AudioContext.Sourcei(m_source, 0x1007, 0);    // AL_LOOPING, AL_FALSE
    m_hasSource = true;
    m_bufferCount = bytes >> 7;
    m_pBuffers = (unsigned int*)CodaMalloc(m_bufferCount * 4);
    if (m_pBuffers == 0)
        return false;
    g_AudioContext.GetError();
    g_AudioContext.GenBuffers(m_bufferCount, m_pBuffers);
    if (g_AudioContext.GetError() != 0) {
        CodaFree(m_pBuffers);
        m_pBuffers = 0;
        return false;
    }
    m_queuedCount = 0;
    m_620 = 0;
    m_624 = 0;
    m_created = true;
    return true;
}

static int fail() { std::printf("00c57800_TEST FAIL\n"); return 1; }
static void reset(CVtable_012b8ed4& v) { std::memset(&v, 0, sizeof(v)); v.m_queuedCount = v.m_620 = v.m_624 = 5; std::memset(&g_AudioContext, 0, sizeof(g_AudioContext)); }

int main()
{
    static const unsigned int ch[5] = { 1, 2, 4, 6, 8 };
    static const long fmt[5] = { 0x1101, 0x1103, 0x1205, 0x120b, 0x1211 };
    CVtable_012b8ed4 v;
    for (int i = 0; i < 5; ++i)
    {
        reset(v);
        if (!v.CreateAndPlay(ch[i], 0x1000)) return fail();
        if (v.m_format != fmt[i] || v.m_blockBytes != 0x100 * ch[i] || v.m_source != 77 || !v.m_hasSource || !v.m_created) return fail();
        if (v.m_bufferCount != 0x20 || g_AudioContext.genCount != 0x20 || v.m_pBuffers[31] != 131) return fail();
        if (g_AudioContext.loopingParam != 0x1007 || g_AudioContext.loopingValue != 0) return fail();
        if (v.m_queuedCount || v.m_620 || v.m_624) return fail();
        std::free(v.m_pBuffers);
    }
    reset(v);
    if (v.CreateAndPlay(3, 0x1000) || v.m_hasSource) return fail();             // unsupported layout
    reset(v); g_AudioContext.enumMissing = 1;
    if (v.CreateAndPlay(6, 0x1000) || v.m_blockBytes != 0x600 || v.m_hasSource) return fail();   // enum not exported
    reset(v); g_AudioContext.errorAfterSources = 0xa001;
    if (v.CreateAndPlay(2, 0x1000) || v.m_hasSource) return fail();
    reset(v); g_failMalloc = 1;
    if (v.CreateAndPlay(2, 0x1000) || !v.m_hasSource || v.m_created) return fail();
    g_failMalloc = 0;
    reset(v); g_AudioContext.errorAfterBuffers = 0xa003;
    if (v.CreateAndPlay(1, 0x400) || v.m_pBuffers != 0 || g_freed != 1 || v.m_created || v.m_queuedCount != 5) return fail();
    std::printf("00c57800_TEST PASS\n");
    return 0;
}
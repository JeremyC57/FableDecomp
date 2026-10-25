// Behaviour test for 00c1c700: CAudioResources::CAudioResources(owner, stream) -- record the owner, flag hardware for device types 1/2/4, store the stream, configure the library and create the voice, then Init
#include <cstdio>

static long g_type; static int g_step = 0; static int g_seq[8]; static long g_flags = -1, g_mode = -1, g_modeArg = -1;
static void* g_callback = (void*)1; static int g_voice;
struct CDefaultSettings { unsigned int Words[0x28]; };

static void QueryDevice(long* type, long* detail) { *type = g_type; *detail = 7; g_seq[g_step++] = 1; }
static void SetStreamFlags(long flags) { g_flags = flags; g_seq[g_step++] = 2; }
static void GetDefaultSettings(CDefaultSettings* out) { out->Words[0] = 0; g_seq[g_step++] = 3; }
static void SetMode(long mode, long arg) { g_mode = mode; g_modeArg = arg; g_seq[g_step++] = 4; }
static void SetCallback(void* callback) { g_callback = callback; g_seq[g_step++] = 5; }
static void* CreateVoice() { g_seq[g_step++] = 6; return &g_voice; }

struct CAudioResources {
    virtual void v00() { }
    void* m_pOwner;
    void* m_pStream;
    void* m_pVoice;
    void* m_pBuffer;
    long m_14;
    bool m_enabled;
    bool m_hardware;
    int m_initCalls;
    void* m_streamAtInit;
    void Init() { ++m_initCalls; m_streamAtInit = m_pStream; g_seq[g_step++] = 7; }
    CAudioResources(void* owner, void* stream)
        : m_pOwner(owner), m_pStream(0), m_pVoice(0), m_pBuffer(0), m_enabled(true), m_hardware(false), m_initCalls(0)
    {
        long type, detail;
        QueryDevice(&type, &detail);
        if (type == 1 || type == 2 || type == 4)
            m_hardware = true;
        m_pStream = stream;
        SetStreamFlags(0);
        CDefaultSettings settings;
        GetDefaultSettings(&settings);
        GetDefaultSettings(&settings);
        SetMode(1, 0);
        SetCallback(0);
        m_pVoice = CreateVoice();
        Init();
    }
};

int main()
{
    int owner, stream;
    long types[6] = { 0, 1, 2, 3, 4, 5 };
    bool hw[6] = { false, true, true, false, true, false };
    for (int i = 0; i < 6; ++i) {
        g_type = types[i]; g_step = 0;
        CAudioResources r(&owner, &stream);
        if (r.m_hardware != hw[i] || !r.m_enabled || r.m_pOwner != &owner || r.m_pStream != &stream || r.m_pBuffer != 0) {
            std::printf("00c1c700_TEST FAIL\n"); return 1;
        }
        if (r.m_pVoice != &g_voice || r.m_initCalls != 1 || r.m_streamAtInit != &stream) { std::printf("00c1c700_TEST FAIL\n"); return 1; }
    }
    int expect[8] = { 1, 2, 3, 3, 4, 5, 6, 7 };
    for (int i = 0; i < 8; ++i) if (g_seq[i] != expect[i]) { std::printf("00c1c700_TEST FAIL\n"); return 1; }
    if (g_flags != 0 || g_mode != 1 || g_modeArg != 0 || g_callback != 0) { std::printf("00c1c700_TEST FAIL\n"); return 1; }
    std::printf("00c1c700_TEST PASS\n");
    return 0;
}
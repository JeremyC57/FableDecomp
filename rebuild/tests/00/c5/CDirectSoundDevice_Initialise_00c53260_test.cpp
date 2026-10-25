#include <cstdio>
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
static const GUID CLSID_DirectSound = { 0x47D4D946, 0x62E8, 0x11CF, { 0x93, 0xBC, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
static const GUID IID_IDirectSound = { 0x279AFA83, 0x4981, 0x11CE, { 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60 } };

static int g_coInitResult = 0, g_createResult = 0, g_initResult = 0, g_coopResult = 0;
static unsigned int g_coInitFlags = 99;
static int g_uninits = 0, g_releases = 0, g_detects = 0;
static unsigned int g_coopLevel = 0; static void* g_coopWindow = 0; static const GUID* g_initDevice = 0;

struct IDirectSound {
    virtual unsigned int Release() { ++g_releases; return 0; }
    virtual int SetCooperativeLevel(void* window, unsigned int level) { g_coopWindow = window; g_coopLevel = level; return g_coopResult; }
    virtual int Initialize(const GUID* device) { g_initDevice = device; return g_initResult; }
};
static IDirectSound g_ds;
static int CoInitializeEx(void* reserved, unsigned int flags) { (void)reserved; g_coInitFlags = flags; return g_coInitResult; }
static int CoCreateInstance(const GUID& clsid, void* outer, unsigned int ctx, const GUID& iid, void** object)
{
    (void)outer;
    if (&clsid != &CLSID_DirectSound || &iid != &IID_IDirectSound || ctx != 1) return -1;
    if (g_createResult == 0) *object = &g_ds;
    return g_createResult;
}
static void CoUninitialize() { ++g_uninits; }

struct CDirectSoundDevice {
    unsigned int m_speakerMode;
    IDirectSound* m_pDirectSound;
    bool m_detectResult;
    bool DetectSpeakerConfig() { ++g_detects; return m_detectResult; }
    void ReleaseDirectSound() { m_pDirectSound->Release(); CoUninitialize(); m_pDirectSound = 0; }
    bool Initialise(const GUID* device, void* window, bool multithreaded)
    {
        int hr = CoInitializeEx(0, multithreaded ? 0 : 2);
        if (hr != 0 && hr != 1)
            return false;
        if (CoCreateInstance(CLSID_DirectSound, 0, 1, IID_IDirectSound, (void**)&m_pDirectSound) != 0) {
            CoUninitialize();
            m_pDirectSound = 0;
            return false;
        }
        if (m_pDirectSound->Initialize(device) < 0) { ReleaseDirectSound(); return false; }
        if (m_pDirectSound->SetCooperativeLevel(window, 2) < 0) { ReleaseDirectSound(); return false; }
        return DetectSpeakerConfig();
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c53260_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    CDirectSoundDevice d; d.m_speakerMode = 0x80000000u; d.m_pDirectSound = 0; d.m_detectResult = true;
    int window = 0;
    CHECK(d.Initialise(&CLSID_DirectSound, &window, true) && g_coInitFlags == 0 && d.m_pDirectSound == &g_ds);
    CHECK(g_coopLevel == 2 && g_coopWindow == &window && g_initDevice == &CLSID_DirectSound && g_detects == 1 && g_uninits == 0);
    d.m_detectResult = false; g_coInitResult = 1;
    CHECK(!d.Initialise(0, &window, false) && g_coInitFlags == 2 && g_detects == 2);
    g_coInitResult = (int)0x80010106;
    CHECK(!d.Initialise(0, &window, false) && g_detects == 2 && g_uninits == 0);
    g_coInitResult = 0; g_createResult = (int)0x80040154;
    CHECK(!d.Initialise(0, &window, false) && d.m_pDirectSound == 0 && g_uninits == 1 && g_releases == 0);
    g_createResult = 0; g_initResult = (int)0x88780078;
    CHECK(!d.Initialise(0, &window, false) && d.m_pDirectSound == 0 && g_uninits == 2 && g_releases == 1);
    g_initResult = 0; g_coopResult = (int)0x80070057;
    CHECK(!d.Initialise(0, &window, false) && d.m_pDirectSound == 0 && g_uninits == 3 && g_releases == 2 && g_detects == 2);
    std::printf("00c53260_TEST PASS\n");
    return 0;
}
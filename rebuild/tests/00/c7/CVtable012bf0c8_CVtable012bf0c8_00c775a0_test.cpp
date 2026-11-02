// Behaviour test for 0x00C775A0 CVtable_012bf0c8::~CVtable_012bf0c8 (libpk DSP settings-block base):
// the settings block is released (and the three block pointers cleared) only when allocated, then ~CDspObject drops the
// shared-table reference.
#include <cstdio>
#include <cstdlib>
static int g_deletes = 0;
static int g_baseDtors = 0;
static void LibFree(void* p) { ++g_deletes; std::free(p); }
struct DspObject { virtual ~DspObject() { ++g_baseDtors; } };
struct SettingsBlockBase : DspObject {
    void* Settings; void* TargetSettings; void* PendingSettings; unsigned int SettingsSize;
    bool Blending; long BlendStep, BlendCountdown, BlendSteps; bool Dirty;
    SettingsBlockBase() : Settings(0), TargetSettings(0), PendingSettings(0), SettingsSize(0), Blending(false), Dirty(false) { }
    void ReleaseBlock()
    {
        if (Settings) {
            LibFree(Settings);
            Settings = 0;
            TargetSettings = 0;
            PendingSettings = 0;
        }
    }
    virtual ~SettingsBlockBase() { ReleaseBlock(); }
};
int main()
{
    SettingsBlockBase* a = new SettingsBlockBase;
    a->SettingsSize = 16;
    a->Settings = std::malloc(48);
    a->TargetSettings = (char*)a->Settings + 16;
    a->PendingSettings = (char*)a->Settings + 32;
    a->ReleaseBlock();
    if (g_deletes != 1 || a->Settings || a->TargetSettings || a->PendingSettings) { std::printf("00c775a0_TEST FAIL\n"); return 1; }
    a->Settings = std::malloc(48);
    DspObject* base = a;
    delete base;
    if (g_deletes != 2 || g_baseDtors != 1) { std::printf("00c775a0_TEST FAIL\n"); return 1; }
    { SettingsBlockBase empty; }
    if (g_deletes != 2 || g_baseDtors != 2) { std::printf("00c775a0_TEST FAIL\n"); return 1; }
    std::printf("00c775a0_TEST PASS\n");
    return 0;
}
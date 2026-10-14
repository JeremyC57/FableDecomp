#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = "";
struct CCharString { void* storage_; ~CCharString() { std::strcat(g_seq, "s"); } };
struct CTCBase { virtual void v00() { } unsigned char _pad04[0x08]; ~CTCBase() { std::strcat(g_seq, "B"); } };
struct CTCEffectOnDie : CTCBase { virtual void v00() { } bool StartedNonRecoverableFadeout; long Effect; long Timer; long FadeOutTimer; CCharString DeathSoundCriteria; bool Pausable; bool Paused; ~CTCEffectOnDie() { } };
int main()
{
    { CTCEffectOnDie e; if ((g_is32 && (char*)&e.DeathSoundCriteria - (char*)&e != 0x1c)) { std::printf("007e0b90_TEST FAIL\n"); return 1; } }
    if (std::strcmp(g_seq, "sB")) { std::printf("007e0b90_TEST FAIL\n"); return 1; }
    std::printf("007e0b90_TEST PASS\n");
    return 0;
}

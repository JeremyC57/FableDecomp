#include <cstdio>
#include <cstring>
// Behaviour model of CTCDParticleEmitter::SetPEmitter (0x006E1080).
static int g_destroySounds = 0, g_createSounds = 0;
struct Emitter { char name[16]; bool killed; };
struct PE {
    char EmitterName[16]; Emitter* PEmitter;
    void SetPEmitter(Emitter* e)
    {
        if (e) {
            if (PEmitter) { ++g_destroySounds; PEmitter->killed = true; }
            PEmitter = e;
            std::strcpy(EmitterName, PEmitter->name);
            ++g_createSounds;
        }
    }
};
int main()
{
    Emitter a = { "fire", false }, b = { "smoke", false };
    PE pe; pe.EmitterName[0] = 0; pe.PEmitter = 0;
    pe.SetPEmitter(0);
    if (g_createSounds) { std::printf("006e1080_TEST FAIL\n"); return 1; }
    pe.SetPEmitter(&a);
    if (pe.PEmitter != &a || std::strcmp(pe.EmitterName, "fire") || g_createSounds != 1 || g_destroySounds) { std::printf("006e1080_TEST FAIL\n"); return 1; }
    pe.SetPEmitter(&b);
    if (!a.killed || g_destroySounds != 1 || std::strcmp(pe.EmitterName, "smoke")) { std::printf("006e1080_TEST FAIL\n"); return 1; }
    std::printf("006e1080_TEST PASS\n");
    return 0;
}
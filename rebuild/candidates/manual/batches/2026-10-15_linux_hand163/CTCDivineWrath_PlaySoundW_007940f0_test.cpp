// behaviour test for 0x007940f0 CTCDivineWrath::PlaySoundW
#include <cstdio>
#include <cstring>
struct Sound { unsigned int lastSound, lastFlags; unsigned int PlaySound(unsigned int s, long a, unsigned int f, long b, long c)
    { lastSound = s; lastFlags = f; return (a == 0 && b == 0 && c == -1) ? s + 100 : 0; } };
struct Thing { Sound* sound; int noises; char lastNoise[16];
    void MakeNoise(const char* n) { ++noises; std::strcpy(lastNoise, n); } };
static unsigned int g_Invalid = 0xffffffffu;
static unsigned int Play(Thing* thing, unsigned int sound, unsigned int flags)
{
    if (thing->sound) { Sound* s = thing->sound; thing->MakeNoise("SIMPLE_NOISE"); return s->PlaySound(sound, 0, flags, 0, -1); }
    return g_Invalid;
}
int main()
{
    Sound s = { 0, 0 };
    Thing t; t.sound = 0; t.noises = 0; t.lastNoise[0] = 0;
    bool ok = Play(&t, 5, 1) == 0xffffffffu && t.noises == 0;
    t.sound = &s;
    ok = ok && Play(&t, 5, 7) == 105 && s.lastFlags == 7 && t.noises == 1 && std::strcmp(t.lastNoise, "SIMPLE_NOISE") == 0;
    if (!ok) { std::printf("007940f0_TEST FAIL\n"); return 1; }
    std::printf("007940f0_TEST PASS\n");
    return 0;
}

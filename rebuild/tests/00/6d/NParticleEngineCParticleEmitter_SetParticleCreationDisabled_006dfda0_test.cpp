// Behaviour test for 0x006DFDA0 CParticleEmitter::SetParticleCreationDisabled: only bit 6 of the 16-bit word at +0x40
// changes; the other flag bits survive.
#include <cstdio>

struct Emitter {
    unsigned char _pad00[0x40];
    unsigned short flags;
    void SetParticleCreationDisabled(bool disabled)
    {
        flags = (unsigned short)((flags & 0xFFBF) | ((disabled ? 1 : 0) << 6));
    }
};

int main()
{
    Emitter e; e.flags = 0xFFFF;
    e.SetParticleCreationDisabled(false);
    if (e.flags != 0xFFBF) { std::printf("006dfda0_TEST FAIL\n"); return 1; }
    e.flags = 0x0201;
    e.SetParticleCreationDisabled(true);
    if (e.flags != 0x0241) { std::printf("006dfda0_TEST FAIL\n"); return 1; }
    std::printf("006dfda0_TEST PASS\n");
    return 0;
}
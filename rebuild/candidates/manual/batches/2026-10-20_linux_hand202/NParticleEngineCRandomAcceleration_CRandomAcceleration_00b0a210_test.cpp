// Behaviour test for NParticleEngine::CRandomAcceleration's constructor (0x00B0A210): six LCG+rotate draws from the
// caller's seed (written back after every step), the low bytes going to IndexX/Y/Z then TimeOffsetX/Y/Z; the seed
// ends at the sixth draw; OldOffset is zeroed; the base constructor runs first.
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static int g_rotates;
static unsigned long GFROR13(unsigned long v) { ++g_rotates; v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
static int g_baseBuilt;
struct CBaseClassNonCopyable { CBaseClassNonCopyable() { ++g_baseBuilt; } virtual ~CBaseClassNonCopyable() {} };
static inline unsigned long NextRandom(unsigned long& seed)
{
    seed = (seed * 0x24A1 + 0x24DF) & 0xffffffffUL;
    seed = GFROR13(seed);
    return seed;
}
class CRandomAcceleration : public CBaseClassNonCopyable {
public:
    unsigned char TimeOffsetX, TimeOffsetY, TimeOffsetZ, IndexX, IndexY, IndexZ;
    C3DVector OldOffset;
    CRandomAcceleration(unsigned long& seed);
};
CRandomAcceleration::CRandomAcceleration(unsigned long& seed)
{
    IndexX = (unsigned char)NextRandom(seed);
    IndexY = (unsigned char)NextRandom(seed);
    IndexZ = (unsigned char)NextRandom(seed);
    TimeOffsetX = (unsigned char)NextRandom(seed);
    TimeOffsetY = (unsigned char)NextRandom(seed);
    TimeOffsetZ = (unsigned char)NextRandom(seed);
    OldOffset = C3DVector(0.0f, 0.0f, 0.0f);
}
int main()
{
    unsigned long seed = 12345, s = 12345, draws[6];
    for (int i = 0; i < 6; ++i) {
        s = (s * 9377 + 9439) & 0xffffffffUL;
        s = ((s >> 13) | (s << 19)) & 0xffffffffUL;
        draws[i] = s & 0xff;
    }
    g_rotates = 0;
    CRandomAcceleration a(seed);
    int bad = 0;
    if (g_baseBuilt != 1 || g_rotates != 6 || seed != s) bad |= 1;
    if (a.IndexX != draws[0] || a.IndexY != draws[1] || a.IndexZ != draws[2]) bad |= 2;
    if (a.TimeOffsetX != draws[3] || a.TimeOffsetY != draws[4] || a.TimeOffsetZ != draws[5]) bad |= 4;
    if (a.OldOffset.X != 0.0f || a.OldOffset.Y != 0.0f || a.OldOffset.Z != 0.0f) bad |= 8;
    if (bad) { std::printf("00b0a210_TEST FAIL\n"); return 1; }
    std::printf("00b0a210_TEST PASS\n");
    return 0;
}

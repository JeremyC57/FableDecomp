// Behaviour test for CObjectAugmentationParticleSet::~CObjectAugmentationParticleSet (0x004E25F1): both arrays' blocks
// are released (malloc'd blocks, so a CRT free must accept them) and empty arrays are skipped; the scalars are left alone.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CDefString { public: long TablePos; };
struct SVector { CDefString* Start; CDefString* Finish; CDefString* End; };
struct SSet {
    float PeriodSecs; long ParticleEffect, ParticleEffectToBlendTo, AttackParticleEffect, AttackParticleEffectToBlendTo;
    SVector StartPoint, EndPoint;
};
class CObjectAugmentationParticleSet {
public:
    unsigned char _raw[0x2c];
    ~CObjectAugmentationParticleSet();
};

int main()
{
    if (sizeof(SSet) != 0x2c) { std::printf("004e25f1_TEST FAIL size\n"); return 1; }
    static SSet s;
    std::memset(&s, 0, sizeof s);
    s.ParticleEffect = 7;
    s.StartPoint.Start = (CDefString*)std::malloc(4 * sizeof(CDefString));
    s.StartPoint.Finish = s.StartPoint.Start + 2; s.StartPoint.End = s.StartPoint.Start + 4;
    s.EndPoint.Start = (CDefString*)std::malloc(2 * sizeof(CDefString));
    s.EndPoint.Finish = s.EndPoint.End = s.EndPoint.Start + 2;
    ((CObjectAugmentationParticleSet*)&s)->~CObjectAugmentationParticleSet();
    if (s.ParticleEffect != 7) { std::printf("004e25f1_TEST FAIL scalars\n"); return 1; }
    static SSet e;                                       // both arrays empty: nothing is freed
    std::memset(&e, 0, sizeof e);
    ((CObjectAugmentationParticleSet*)&e)->~CObjectAugmentationParticleSet();
    std::printf("004e25f1_TEST PASS\n");
    return 0;
}

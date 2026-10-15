#include <cstdio>
// Behaviour test for CObjectAugmentationParticleSet::CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet&) @ 0x004EC46D
static int g_arrayCopies = 0;
class CDefString { public: long m_index; };
class CArrayDefString {
public:
    CDefString* m_pData; unsigned long m_size; unsigned long m_capacity;
    CArrayDefString() : m_pData(0), m_size(0), m_capacity(0) {}
    CArrayDefString(const CArrayDefString& other) : m_pData(other.m_pData), m_size(other.m_size), m_capacity(other.m_size) { ++g_arrayCopies; }
};
class CObjectAugmentationParticleSet {
public:
    float PeriodSecs; long ParticleEffect; long ParticleEffectToBlendTo; long AttackParticleEffect; long AttackParticleEffectToBlendTo;
    CArrayDefString StartPoint; CArrayDefString EndPoint;
    CObjectAugmentationParticleSet() {}
    CObjectAugmentationParticleSet(const CObjectAugmentationParticleSet& other)
        : PeriodSecs(other.PeriodSecs), ParticleEffect(other.ParticleEffect), ParticleEffectToBlendTo(other.ParticleEffectToBlendTo),
          AttackParticleEffect(other.AttackParticleEffect), AttackParticleEffectToBlendTo(other.AttackParticleEffectToBlendTo),
          StartPoint(other.StartPoint), EndPoint(other.EndPoint) {}
};
int main()
{
    CDefString names[3];
    CObjectAugmentationParticleSet a;
    a.PeriodSecs = 2.5f; a.ParticleEffect = 1; a.ParticleEffectToBlendTo = 2; a.AttackParticleEffect = 3; a.AttackParticleEffectToBlendTo = 4;
    a.StartPoint.m_pData = names; a.StartPoint.m_size = 2; a.EndPoint.m_pData = names + 2; a.EndPoint.m_size = 1;
    CObjectAugmentationParticleSet b(a);
    if (b.PeriodSecs != 2.5f || b.ParticleEffect != 1 || b.ParticleEffectToBlendTo != 2 || b.AttackParticleEffect != 3 ||
        b.AttackParticleEffectToBlendTo != 4 || b.StartPoint.m_pData != names || b.StartPoint.m_size != 2 ||
        b.EndPoint.m_pData != names + 2 || b.EndPoint.m_size != 1 || g_arrayCopies != 2) { std::printf("004ec46d_TEST FAIL\n"); return 1; }
    std::printf("004ec46d_TEST PASS\n");
    return 0;
}

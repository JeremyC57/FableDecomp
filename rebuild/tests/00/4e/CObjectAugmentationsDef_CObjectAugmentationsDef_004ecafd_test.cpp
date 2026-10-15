#include <cstdio>
#include <cstdlib>
#include <cstring>
// CObjectAugmentationsDef::~CObjectAugmentationsDef: members destroyed in reverse order, then the def base chain
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CCharString { ~CCharString() { Log("S"); } };
struct CBase_009fc550 { virtual void v00() { } ~CBase_009fc550() { Log("R"); } };
struct CDefPointeeBase : CBase_009fc550 { virtual void v00() { } ~CDefPointeeBase() { } };
struct CAugmentationParticleMap { ~CAugmentationParticleMap() { Log("P"); } };
struct CAugmentationSoundMap { long tag; ~CAugmentationSoundMap() { Log(tag == 1 ? "I" : "L"); } };
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd; long tag;
    CLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), tag(0) { }
    ~CLongVector() { if (m_pFirst) { std::free(m_pFirst); Log(tag == 1 ? "a" : "c"); } }
};
struct CObjectAugmentationsDef : CDefPointeeBase {
    long NumberOfSlots; long MaximumNumberOfSlots;
    CAugmentationParticleMap AugmentationParticles; unsigned long EmptySlotName;
    CLongVector InitialAugmentations; CLongVector SlotUpgradeCosts;
    CAugmentationSoundMap InitSounds; CAugmentationSoundMap LoopingSounds;
    long Effects[6];
    CCharString WeaponTrailAttachmentPoint;
    virtual void v00() { }
    ~CObjectAugmentationsDef() { }
};
int main()
{
    {
        CObjectAugmentationsDef def;
        def.InitSounds.tag = 1; def.LoopingSounds.tag = 2;
        def.InitialAugmentations.tag = 1; def.InitialAugmentations.m_pFirst = (long*)std::malloc(8);
        def.SlotUpgradeCosts.tag = 2; def.SlotUpgradeCosts.m_pFirst = (long*)std::malloc(8);
    }
    // string, looping sounds, init sounds, slot costs, initial augmentations, particles, root base
    if (std::strcmp(g_log, "SLIcaPR") != 0) { std::printf("004ecafd_TEST FAIL\n"); return 1; }
    std::printf("004ecafd_TEST PASS\n");
    return 0;
}
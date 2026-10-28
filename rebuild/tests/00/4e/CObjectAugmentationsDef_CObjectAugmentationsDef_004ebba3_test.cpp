// Behaviour test for 0x004EBBA3 CObjectAugmentationsDef::CObjectAugmentationsDef: the base constructor runs, the particle map, both vectors and both sound maps are built empty in member order, then the trail attachment-point string
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
inline void* operator new(tsize, void* p) { return p; }
static int g_base = 0;
static int g_vecs = 0;
struct Vec { int* start; int* finish; int* eos; Vec() : start(0), finish(0), eos(0) { ++g_vecs; } };
class CSubDefClassBase { public: CSubDefClassBase() { ++g_base; } virtual ~CSubDefClassBase() { } };
class CObjectAugmentationsDef : public CSubDefClassBase { public:
    CObjectAugmentationsDef();
    struct Map { int order; Map(); } AugmentationParticles;
    Vec InitialAugmentations;
    Vec SlotUpgradeCosts;
    Map InitSounds;
    Map LoopingSounds;
    int FireProjectileTrailEffect;
    struct Str { int order; Str(); } WeaponTrailAttachmentPoint;
};
CObjectAugmentationsDef::CObjectAugmentationsDef() { }
static int g_order = 0;
CObjectAugmentationsDef::Map::Map() : order(++g_order) { }
CObjectAugmentationsDef::Str::Str() : order(++g_order) { }
int main()
{
    static unsigned char raw[sizeof(CObjectAugmentationsDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CObjectAugmentationsDef* d = new (raw) CObjectAugmentationsDef;
    CHECK(g_base == 1);
    CHECK(d->AugmentationParticles.order == 1);
    CHECK(d->InitialAugmentations.start == 0 && d->InitialAugmentations.eos == 0);
    CHECK(d->SlotUpgradeCosts.finish == 0);
    CHECK(d->InitSounds.order == 2);
    CHECK(d->LoopingSounds.order == 3);
    CHECK(d->FireProjectileTrailEffect == (int)0xCDCDCDCD);
    CHECK(d->WeaponTrailAttachmentPoint.order == 4);
    if (fails == 0) printf("004ebba3_TEST PASS\n"); else printf("004ebba3_TEST FAIL\n");
    return fails;
}
// Behaviour test for 0x0045647D CEntitySoundDef::CEntitySoundDef: the base constructor runs, both vector maps of the sound map start empty and clean (Dirty false), then the sound identifier string is constructed
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
class CEntitySoundDef : public CSubDefClassBase { public:
    CEntitySoundDef();
    struct VMap { Vec Data; bool Dirty; VMap() : Dirty(false) { } } SoundMap;
    VMap CriteriaMap;
    struct Str { int made; Str() : made(1) { } } SoundIdentifier;
    float VoicePitchOverride;
};
CEntitySoundDef::CEntitySoundDef() { }
int main()
{
    static unsigned char raw[sizeof(CEntitySoundDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CEntitySoundDef* d = new (raw) CEntitySoundDef;
    CHECK(g_base == 1);
    CHECK(d->SoundMap.Data.start == 0 && d->SoundMap.Data.eos == 0 && d->SoundMap.Dirty == false);
    CHECK(d->CriteriaMap.Data.finish == 0 && d->CriteriaMap.Dirty == false);
    CHECK(d->SoundIdentifier.made == 1);
    if (fails == 0) printf("0045647d_TEST PASS\n"); else printf("0045647d_TEST FAIL\n");
    return fails;
}
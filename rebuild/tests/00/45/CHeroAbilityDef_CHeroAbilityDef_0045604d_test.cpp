// Behaviour test for 0x0045604D CHeroAbilityDef::CHeroAbilityDef: the base constructor runs, the three per-level vectors start empty and the dummy-object def index is 0; the other scalars are untouched
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
class CParentDefClassBase { public: CParentDefClassBase() { ++g_base; } virtual ~CParentDefClassBase() { } };
class CHeroAbilityDef : public CParentDefClassBase { public:
    CHeroAbilityDef();
    int Ability;
    Vec LevDescription;
    Vec StaminaCost;
    Vec ExperienceCostsToUpgrade;
    struct CDefIndex { int Val; CDefIndex() : Val(0) { } } DummyObject;
};
CHeroAbilityDef::CHeroAbilityDef() { }
int main()
{
    static unsigned char raw[sizeof(CHeroAbilityDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CHeroAbilityDef* d = new (raw) CHeroAbilityDef;
    CHECK(g_base == 1);
    CHECK(d->Ability == (int)0xCDCDCDCD);
    CHECK(d->LevDescription.start == 0 && d->LevDescription.finish == 0 && d->LevDescription.eos == 0);
    CHECK(d->StaminaCost.start == 0 && d->StaminaCost.eos == 0);
    CHECK(d->ExperienceCostsToUpgrade.finish == 0 && d->ExperienceCostsToUpgrade.eos == 0);
    CHECK(d->DummyObject.Val == 0);
    if (fails == 0) printf("0045604d_TEST PASS\n"); else printf("0045604d_TEST FAIL\n");
    return fails;
}
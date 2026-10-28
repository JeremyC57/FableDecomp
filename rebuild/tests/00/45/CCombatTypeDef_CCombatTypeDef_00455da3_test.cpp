// Behaviour test for 0x00455DA3 CCombatTypeDef::CCombatTypeDef: the base constructor runs, the three vectors start empty, then the attack-pattern map is built by its out-of-line constructor (after the vectors)
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
class CCombatTypeDef : public CParentDefClassBase { public:
    CCombatTypeDef();
    Vec DefenderCombatRingOccupationNumbers;
    Vec CombatSequence;
    Vec CombatSequenceDef;
    float PreferredCombatDistance;
    struct Map { int built; int vecsDone; Map(); } AttackPatterns;
};
CCombatTypeDef::CCombatTypeDef() { }
CCombatTypeDef::Map::Map() : built(1), vecsDone(g_vecs == 3 ? 1 : 0) { }
int main()
{
    static unsigned char raw[sizeof(CCombatTypeDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CCombatTypeDef* d = new (raw) CCombatTypeDef;
    CHECK(g_base == 1);
    CHECK(d->DefenderCombatRingOccupationNumbers.start == 0 && d->DefenderCombatRingOccupationNumbers.eos == 0 && g_vecs == 3);
    CHECK(d->CombatSequence.finish == 0);
    CHECK(d->CombatSequenceDef.start == 0);
    CHECK(d->AttackPatterns.built == 1 && d->AttackPatterns.vecsDone == 1);
    if (fails == 0) printf("00455da3_TEST PASS\n"); else printf("00455da3_TEST FAIL\n");
    return fails;
}
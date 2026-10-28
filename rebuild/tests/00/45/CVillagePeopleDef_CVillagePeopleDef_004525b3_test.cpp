// Behaviour test for 0x004525B3 CVillagePeopleDef::CVillagePeopleDef: the base constructor runs, then both guard-respawn vectors start empty (start = finish = end = 0); the scalar members are left as they were
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
class CVillagePeopleDef : public CSubDefClassBase { public:
    CVillagePeopleDef();
    Vec GuardRespawnType;
    Vec GuardRespawnQuantity;
    int GuardRespawnMaxRenownBasedOffset;
};
CVillagePeopleDef::CVillagePeopleDef() { }
int main()
{
    static unsigned char raw[sizeof(CVillagePeopleDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CVillagePeopleDef* d = new (raw) CVillagePeopleDef;
    CHECK(g_base == 1);
    CHECK(d->GuardRespawnType.start == 0 && d->GuardRespawnType.finish == 0 && d->GuardRespawnType.eos == 0);
    CHECK(d->GuardRespawnQuantity.start == 0 && d->GuardRespawnQuantity.finish == 0 && d->GuardRespawnQuantity.eos == 0);
    CHECK(d->GuardRespawnMaxRenownBasedOffset == (int)0xCDCDCDCD);
    if (fails == 0) printf("004525b3_TEST PASS\n"); else printf("004525b3_TEST FAIL\n");
    return fails;
}
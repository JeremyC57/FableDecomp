// Behaviour test for 0x004E1195 CTavernGameCardBaseDef::CTavernGameCardBaseDef: the base constructor runs, then the card-order and card-pack vectors start empty
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
class CTavernGameCardBaseDef : public CSubDefClassBase { public:
    CTavernGameCardBaseDef();
    Vec CardOrders;
    Vec CardPack;
};
CTavernGameCardBaseDef::CTavernGameCardBaseDef() { }
int main()
{
    static unsigned char raw[sizeof(CTavernGameCardBaseDef) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CTavernGameCardBaseDef* d = new (raw) CTavernGameCardBaseDef;
    CHECK(g_base == 1);
    CHECK(d->CardOrders.start == 0 && d->CardOrders.finish == 0 && d->CardOrders.eos == 0);
    CHECK(d->CardPack.start == 0 && d->CardPack.finish == 0 && d->CardPack.eos == 0);
    if (fails == 0) printf("004e1195_TEST PASS\n"); else printf("004e1195_TEST FAIL\n");
    return fails;
}
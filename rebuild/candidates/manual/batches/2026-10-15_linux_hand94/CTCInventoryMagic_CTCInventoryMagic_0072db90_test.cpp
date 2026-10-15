#include <cstdio>
#include <new>
// Behaviour test for 0x0072DB90: both bases built (thing passed on), the handle vector emptied.
struct CThing { int id; };
static CThing* g_baseThing = 0; static int g_secondBase = 0;
struct CTCInventoryBase { virtual void v00() {} CTCInventoryBase(CThing* thing) { g_baseThing = thing; } };
namespace NHeroInformationScreens { struct CBase { virtual void v01() {} CBase() { ++g_secondBase; } }; }
struct CEnginePrimitiveHandle;
struct SPrimitiveHandleVector {
    CEnginePrimitiveHandle* _M_start; CEnginePrimitiveHandle* _M_finish; CEnginePrimitiveHandle* _M_end_of_storage;
    SPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CTCInventoryMagic : CTCInventoryBase, NHeroInformationScreens::CBase {
    SPrimitiveHandleVector InventoryMagicPrimitiveHandles;
    virtual void v00() {}
    CTCInventoryMagic(CThing* thing);
};
CTCInventoryMagic::CTCInventoryMagic(CThing* thing)
    : CTCInventoryBase(thing)
{
}
int main()
{
    CThing t;
    unsigned char raw[sizeof(CTCInventoryMagic) + 16];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CTCInventoryMagic* m = new (raw) CTCInventoryMagic(&t);
    if (g_baseThing != &t || g_secondBase != 1 || m->InventoryMagicPrimitiveHandles._M_start != 0 ||
        m->InventoryMagicPrimitiveHandles._M_finish != 0 || m->InventoryMagicPrimitiveHandles._M_end_of_storage != 0) {
        std::printf("0072db90_TEST FAIL\n"); return 1;
    }
    std::printf("0072db90_TEST PASS\n");
    return 0;
}

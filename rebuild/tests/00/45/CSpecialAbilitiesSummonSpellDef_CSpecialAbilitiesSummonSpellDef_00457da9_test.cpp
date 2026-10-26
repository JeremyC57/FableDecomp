// Behaviour test for 00457da9: body copied from src/00457da9.cpp (CSpecialAbilitiesSummonSpellDef ctor), callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_strCtor = 0;
class CParentDefClassBase { public:
    virtual void v00() {}
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCtor; }
};
template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};
class CDefString { public: long m_index; CDefString() : m_index(-1) { } };
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strCtor; } };
class CSpecialAbilitiesSummonSpellDef : public CParentDefClassBase { public:
    CRetailVector<float> TimeOfSummon; CDefString BaseSummonCreatureName; float FadeInTime; float FadeOutTime; long ResetCreatureManaCost;
    CRetailVector<long> SummonTintColour; CCharString EyeLeftDummyPointName; CCharString EyeRightDummyPointName;
    virtual void v00() {}
    CSpecialAbilitiesSummonSpellDef();
};
CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef()
    : CParentDefClassBase()
{
}
int main()
{
    unsigned char raw[sizeof(CSpecialAbilitiesSummonSpellDef)]; memset(raw, 0xAA, sizeof(raw));
    CSpecialAbilitiesSummonSpellDef* def = new (raw) CSpecialAbilitiesSummonSpellDef();
    CHECK(g_baseCtor == 1 && g_strCtor == 2);
    CHECK(def->TimeOfSummon.first == 0 && def->TimeOfSummon.last == 0 && def->TimeOfSummon.end.p == 0);
    CHECK(def->SummonTintColour.first == 0 && def->SummonTintColour.last == 0 && def->SummonTintColour.end.p == 0);
    CHECK(def->BaseSummonCreatureName.m_index == -1 && def->EyeLeftDummyPointName.PStringData == 0 && def->EyeRightDummyPointName.PStringData == 0);
    CHECK(def->ResetCreatureManaCost == (long)0xAAAAAAAA);
    if (fails == 0) printf("00457da9_TEST PASS\n"); else printf("00457da9_TEST FAIL\n");
    return fails;
}
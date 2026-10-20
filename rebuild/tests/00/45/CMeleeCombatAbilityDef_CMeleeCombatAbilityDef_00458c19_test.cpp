// Behaviour test for 00458c19: body copied from src/00458c19.cpp (CMeleeCombatAbilityDef ctor), callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_statsCtor = 0;
struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _base[0x38];
    CParentDefClassBase() { ++g_baseCtor; memset(_base, 0x77, sizeof(_base)); }
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
struct CDefString { long m_index; CDefString() : m_index(-1) { } };
struct CHeroAbilityStatPackage { long m_stats[4]; CHeroAbilityStatPackage() { ++g_statsCtor; m_stats[0] = m_stats[1] = m_stats[2] = m_stats[3] = 9; } };
struct CMeleeCombatAbilityDef : CParentDefClassBase {
    CDefString FlourishAnimName; CDefString FlourishResponseAnimName; float DamageMultiplier; bool Flourish; bool BlockCounter; long AbilityButtonGraphicOverride;
    CRetailVector<long> ValidWeaponClasses; bool Decapitate; long KnockDownEffects; bool EvadeAllHits; bool CauseRecoil; CHeroAbilityStatPackage StatsNeeded;
    long ComboMultiplierLowerBound; long ComboMultiplierUpperBound;
    virtual void v00() {}
    CMeleeCombatAbilityDef();
};
CMeleeCombatAbilityDef::CMeleeCombatAbilityDef()
    : CParentDefClassBase()
{
}
int main()
{
    unsigned char raw[sizeof(CMeleeCombatAbilityDef)]; memset(raw, 0xAA, sizeof(raw));
    CMeleeCombatAbilityDef* def = new (raw) CMeleeCombatAbilityDef();
    CHECK(g_baseCtor == 1 && g_statsCtor == 1);
    CHECK(def->FlourishAnimName.m_index == -1 && def->FlourishResponseAnimName.m_index == -1);
    CHECK(def->ValidWeaponClasses.first == 0 && def->ValidWeaponClasses.last == 0 && def->ValidWeaponClasses.end.p == 0);
    CHECK(def->StatsNeeded.m_stats[3] == 9 && def->_base[0] == 0x77);
    CHECK(def->KnockDownEffects == (long)0xAAAAAAAA && def->ComboMultiplierUpperBound == (long)0xAAAAAAAA);   // not touched by the ctor
    if (fails == 0) printf("00458c19_TEST PASS\n"); else printf("00458c19_TEST FAIL\n");
    return fails;
}
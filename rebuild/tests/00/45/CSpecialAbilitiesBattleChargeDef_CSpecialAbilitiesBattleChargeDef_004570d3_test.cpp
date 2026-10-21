// Behaviour test for 004570d3: body copied from src/004570d3.cpp (CSpecialAbilitiesBattleChargeDef ctor), callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0;
struct CParentDefClassBase { virtual void v00() {} unsigned char _base[0x38]; CParentDefClassBase() { ++g_baseCtor; } };
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
struct CSpecialAbilitiesBattleChargeDef : CParentDefClassBase {
    float RunSpeed; CRetailVector<float> DashDistance; CRetailVector<float> RadiusOfEffect; CRetailVector<float> BattleChargeDamage;
    float BattleChargeCombatMultiplierFactor; float BattleChargeRadialStartSeconds; float BattleChargeRadialEndSeconds; float BattleChargeRadialThreshold;
    float BattleChargeRadialLength; float BattleChargeRadialIntensity; float BattleChargeRadialFade; float BattleChargeRadialRange; float BattleChargeRadialTwist;
    float PoseDelaySecs; float ChargeDelay; CRetailVector<long> NumPeopleHit; float DragonExtraGumphRadiusMultiplier; float DragonExtraGumphRadius;
    virtual void v00() {}
    CSpecialAbilitiesBattleChargeDef();
};
CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef()
    : CParentDefClassBase()
{
}
int main()
{
    unsigned char raw[sizeof(CSpecialAbilitiesBattleChargeDef)]; memset(raw, 0xAA, sizeof(raw));
    CSpecialAbilitiesBattleChargeDef* def = new (raw) CSpecialAbilitiesBattleChargeDef();
    CHECK(g_baseCtor == 1);
    CHECK(def->DashDistance.first == 0 && def->DashDistance.last == 0 && def->DashDistance.end.p == 0);
    CHECK(def->RadiusOfEffect.first == 0 && def->RadiusOfEffect.end.p == 0 && def->BattleChargeDamage.last == 0 && def->BattleChargeDamage.end.p == 0);
    CHECK(def->NumPeopleHit.first == 0 && def->NumPeopleHit.last == 0 && def->NumPeopleHit.end.p == 0);
    CHECK(*(unsigned long*)&def->RunSpeed == 0xAAAAAAAA && *(unsigned long*)&def->DragonExtraGumphRadius == 0xAAAAAAAA);
    if (fails == 0) printf("004570d3_TEST PASS\n"); else printf("004570d3_TEST FAIL\n");
    return fails;
}
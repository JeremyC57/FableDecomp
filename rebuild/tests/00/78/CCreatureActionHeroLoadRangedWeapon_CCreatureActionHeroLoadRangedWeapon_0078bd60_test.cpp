#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
static int g_baseCopies = 0;
struct CActionBase {
    virtual ~CActionBase() { }
    long m_baseWord;
    CActionBase() : m_baseWord(0) { }
    CActionBase(const CActionBase& other) : m_baseWord(other.m_baseWord) { ++g_baseCopies; }
};
struct CCreatureAction_HeroLoadRangedWeapon : CActionBase {
    CIntelligentPointer PWeapon;
    CIntelligentPointer PAmmo;
    CCreatureAction_HeroLoadRangedWeapon() { }
    CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& other);
};
CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& other)
    : CActionBase(other)
    , PWeapon(other.PWeapon)
    , PAmmo(other.PAmmo)
{
}
int main()
{
    CBaseObject a; a.id = 1;
    CBaseObject b; b.id = 2;
    (void)a; (void)b;
    CCreatureAction_HeroLoadRangedWeapon src;
    src.m_baseWord = 77;
    src.PWeapon.SetPItem(&a);
    src.PAmmo.SetPItem(&b);
    CCreatureAction_HeroLoadRangedWeapon copy(src);
    bool ok = g_baseCopies == 1 && copy.m_baseWord == 77 && copy.PWeapon.GetPItem() == &a && copy.PAmmo.GetPItem() == &b;
    if (!ok) { std::printf("0078bd60_TEST FAIL\n"); return 1; }
    std::printf("0078bd60_TEST PASS\n");
    return 0;
}
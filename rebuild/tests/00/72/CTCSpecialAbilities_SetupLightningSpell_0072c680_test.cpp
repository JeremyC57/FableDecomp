// 0072c680 behaviour test for CTCSpecialAbilities::SetupLightningSpell
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std {
template <unsigned int _Nb>
class bitset {
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) { puts("bitset"); abort(); }
        return _Unchecked_test(__pos);
    }
    void set(unsigned int __pos) { _M_w[_S_whichword(__pos)] |= _S_maskbit(__pos); }
};
}
class CTCParamsBase;
static int g_ctorCalls = 0, g_dtorCalls = 0, g_addCalls = 0;
static char g_lastName[64]; static bool g_lastActivate = false; static const CTCParamsBase* g_lastParams = (const CTCParamsBase*)1;
class CCharString {
public:
    CCharString(const char* text, long length = -1) { ++g_ctorCalls; m_pStringData = (void*)text; (void)length; }
    ~CCharString() { ++g_dtorCalls; }
    void* m_pStringData;
};
class CTCBase { public: int Tag; CTCBase() : Tag(0) {} };
enum ETCInterfaceType { TC_NONE = 0 };
static CTCBase g_component;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params)
    {
        ++g_addCalls; strcpy(g_lastName, (const char*)name.m_pStringData); g_lastActivate = activate; g_lastParams = params;
        return &g_component;
    }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
};
class CThingCreatureBase : public CThing { public: };
class CTCSpecialAbilities {
public:
    enum EWEAPON_STATE { WEAPON_STATE_NONE = 0 };
    void*         __vftable;
    CThing*       ParentThing;
    bool          ToBeRemoved;
    bool          IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    bool          AbilityMutexLock;
    bool          UsingBed;
    long          CurrentAbility;
    void*         UndeadFollowers;
    CThingCreatureBase* Creature;
    bool SetupLightningSpell();
};
static CThingCreatureBase* g_calcArg = 0;
class CTCWillLightning : public CTCBase {
public:
    CTCSpecialAbilities::EWEAPON_STATE WeaponState;
    void SetWeaponState(CTCSpecialAbilities::EWEAPON_STATE state) { WeaponState = state; }
};
static CTCWillLightning g_lightning;
CTCSpecialAbilities::EWEAPON_STATE __fastcall CalculateWeaponState(CThingCreatureBase* creature)
{
    g_calcArg = creature; return (CTCSpecialAbilities::EWEAPON_STATE)3;
}

bool CTCSpecialAbilities::SetupLightningSpell()
{
    if (!Creature->HasTC((ETCInterfaceType)0x94)) {
        CTCWillLightning* lightning = (CTCWillLightning*)(CTCBase*)&g_lightning; Creature->AddTC("CTCWillLightning", true, 0);
        lightning->SetWeaponState(CalculateWeaponState(Creature));
    }
    return true;
}

static bool run(bool present, bool& result)
{
    CThingCreatureBase creature; CThing parent;
    memset(creature.UsingComponents._M_w, 0, sizeof(creature.UsingComponents._M_w));
    memset(parent.UsingComponents._M_w, 0, sizeof(parent.UsingComponents._M_w));
    CTCSpecialAbilities sa; sa.ParentThing = &parent; sa.Creature = &creature; sa.CurrentAbility = 0;
    if (present) creature.UsingComponents.set(0x94);
    g_ctorCalls = g_dtorCalls = g_addCalls = 0; g_lastName[0] = 0; g_lastActivate = false; g_lastParams = (const CTCParamsBase*)1;
    result = sa.SetupLightningSpell();
    return true;
}
int main()
{
    bool ok = true, r = false;
    run(false, r);
    ok = ok && r && g_addCalls == 1 && g_ctorCalls == 1 && g_dtorCalls == 1 && strcmp(g_lastName, "CTCWillLightning") == 0 && g_lastActivate && g_lastParams == 0;
    ok = ok && g_lightning.WeaponState == 3 && g_calcArg != 0;
    run(true, r);
    ok = ok && r == true && g_addCalls == 0 && g_ctorCalls == 0;
    printf(ok ? "0072c680_TEST PASS\n" : "0072c680_TEST FAIL\n");
    return ok ? 0 : 1;
}
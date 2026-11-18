// 0072aaf0 behaviour test for CTCSpecialAbilities::SetupMultiStrike: when the parent thing has no TCI_MULTI_STRIKE (135 = word +0x30 bit 7) component, AddTC("CTCMultiStrike", true, 0) and return true; else false with no call
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
static char g_lastName[64]; static long g_lastLength = 0; static const void* g_lastThis = 0; static bool g_lastActivate = false; static const CTCParamsBase* g_lastParams = (const CTCParamsBase*)1;
class CCharString {
public:
    CCharString(const char* text, long length = -1) { ++g_ctorCalls; m_pStringData = (void*)text; g_lastLength = length; }
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
        ++g_addCalls; g_lastThis = this; strcpy(g_lastName, (const char*)name.m_pStringData); g_lastActivate = activate; g_lastParams = params;
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
    bool SetupMultiStrike();
};

bool CTCSpecialAbilities::SetupMultiStrike()
{
    if (!ParentThing->HasTC((ETCInterfaceType)0x87)) {
        ParentThing->AddTC("CTCMultiStrike", true, 0);
        return true;
    }
    return false;
}

static bool g_thisOk = false;
static bool run(bool present, bool& result, unsigned int other = 0)
{
    CThingCreatureBase creature; CThing parent;
    memset(creature.UsingComponents._M_w, 0, sizeof(creature.UsingComponents._M_w));
    memset(parent.UsingComponents._M_w, 0, sizeof(parent.UsingComponents._M_w));
    CTCSpecialAbilities sa; sa.ParentThing = &parent; sa.Creature = &creature; sa.CurrentAbility = 0;
    if (present) parent.UsingComponents.set(0x87);
    if (other) parent.UsingComponents.set(other);
    creature.UsingComponents.set(0x87);
    g_ctorCalls = g_dtorCalls = g_addCalls = 0; g_lastLength = 0; g_lastThis = 0; g_lastName[0] = 0; g_lastActivate = false; g_lastParams = (const CTCParamsBase*)1;
    result = sa.SetupMultiStrike();
    g_thisOk = (g_lastThis == (const void*)&parent);   // AddTC's receiver is ParentThing (+0x04)
    return true;
}
int main()
{
    bool ok = true, r = false;
    run(false, r);
    ok = ok && r && g_addCalls == 1 && g_ctorCalls == 1 && g_dtorCalls == 1 && strcmp(g_lastName, "CTCMultiStrike") == 0 && g_lastActivate && g_lastParams == 0
         && g_thisOk && g_lastLength == -1;   // AddTC on ParentThing (+0x04), CCharString(text, -1)
    // neighbouring interfaces present (134 / 136 = MultiArrow): still absent -> added
    run(false, r, 0x86);
    ok = ok && r && g_addCalls == 1;
    run(false, r, 0x88);
    ok = ok && r && g_addCalls == 1;
    run(true, r);
    ok = ok && r == false && g_addCalls == 0 && g_ctorCalls == 0 && g_dtorCalls == 0;
    printf(ok ? "0072aaf0_TEST PASS\n" : "0072aaf0_TEST FAIL\n");
    return ok ? 0 : 1;
}

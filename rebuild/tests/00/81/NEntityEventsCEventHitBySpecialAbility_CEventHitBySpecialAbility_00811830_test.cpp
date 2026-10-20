// Behaviour test for 00811830: body copied from src/00811830.cpp (NEntityEvents::CEventHitBySpecialAbility ctor), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCtors = 0, g_strDtors = 0;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length) : PStringData((void*)text) { ++g_strCtors; CHECK(length == -1 && strcmp(text, "HERO_ABILITY") == 0); }
    ~CCharString() { ++g_strDtors; }
};
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CHeroAbilityDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    long Ability;
    unsigned char _pad40[0x94 - 0x40];
    bool Aggressive;
};
static CHeroAbilityDef g_defs[4]; static long g_count = 0; static int g_countCalls = 0; static int g_getDef = 0; static long g_lastIndex = -1;
struct CDefinitionManager {
    static CDefinitionManager& __fastcall Get();
    long GetNumberOfDefsOfType(const CCharString& typeName) { ++g_countCalls; return g_count; }
    bool GetDef_005f89f2(const CCharString& typeName, long index, CDefPointer<CHeroAbilityDef>& out) { ++g_getDef; g_lastIndex = index; ++g_defs[index].RefCount; out.Object = &g_defs[index]; return true; }
};
static CDefinitionManager g_manager;
CDefinitionManager& __fastcall CDefinitionManager::Get() { return g_manager; }
struct CHitParameters { int tag; };
struct CCPPointerInfo;
namespace NEntityEvents {
static int g_baseCtor = 0; static const CHitParameters* g_baseParams = 0;
struct CEventHitBase {
    virtual void v00() {}
    long WorldFrame;
    CHitParameters* PHitParams_Data;
    CCPPointerInfo* PHitParams_Info;
    CEventHitBase(const CHitParameters& params) : WorldFrame(77), PHitParams_Data((CHitParameters*)&params), PHitParams_Info(0) { ++g_baseCtor; g_baseParams = &params; }
};
struct CEventHitBySpecialAbility : CEventHitBase {
    long Ability;
    bool Aggressive;
    virtual void v00() {}
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);
};
CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& params, long ability)
    : CEventHitBase(params), Ability(ability), Aggressive(true)
{
    CDefinitionManager& manager = CDefinitionManager::Get();
    long count;
    {
        CCharString typeName("HERO_ABILITY", -1);
        count = manager.GetNumberOfDefsOfType(typeName);
    }
    for (long i = 1; i < count; ++i) {
        CDefPointer<CHeroAbilityDef> def;
        {
            CCharString typeName("HERO_ABILITY", -1);
            manager.GetDef_005f89f2(typeName, i, def);
        }
        if (def->Ability == Ability) {
            Aggressive = def->Aggressive;
            return;
        }
    }
}
}
int main()
{
    for (int i = 0; i < 4; ++i) { g_defs[i].RefCount = 1; g_defs[i].Ability = 10 + i; g_defs[i].Aggressive = (i != 2); }
    CHitParameters params = { 5 };
    // no defs: aggressive by default, the count asked once
    g_count = 0;
    { NEntityEvents::CEventHitBySpecialAbility e(params, 12); CHECK(e.Ability == 12 && e.Aggressive && e.WorldFrame == 77 && NEntityEvents::g_baseParams == &params); CHECK(g_countCalls == 1 && g_getDef == 0 && g_strCtors == 1 && g_strDtors == 1); }
    // defs 1..3 searched (index 0 skipped); ability 12 is def 2, not aggressive; search stops there
    g_count = 4;
    { NEntityEvents::CEventHitBySpecialAbility e(params, 12); CHECK(!e.Aggressive && g_getDef == 2 && g_lastIndex == 2); CHECK(g_defs[1].RefCount == 1 && g_defs[2].RefCount == 1 && g_destroyed == 0); }
    // ability 13 is def 3, aggressive
    { NEntityEvents::CEventHitBySpecialAbility e(params, 13); CHECK(e.Aggressive && g_getDef == 5 && g_lastIndex == 3); }
    // unknown ability: every def visited, stays aggressive
    { NEntityEvents::CEventHitBySpecialAbility e(params, 99); CHECK(e.Aggressive && g_getDef == 8); }
    CHECK(g_strCtors == g_strDtors);
    // a def held only by the lookup is destroyed by the pointer
    g_defs[1].RefCount = 0; { NEntityEvents::CEventHitBySpecialAbility e(params, 11); CHECK(g_destroyed == 1 && e.Aggressive); }
    if (fails == 0) printf("00811830_TEST PASS\n"); else printf("00811830_TEST FAIL\n");
    return fails;
}
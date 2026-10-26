#include <cstdio>
#include <cstring>
// Model of CCreatureAction_OnGenerate constructor @ 0x0074DBB0: base(owner, 250, GetSoundPair(owner, static_cast<const char*>(0)))

class CThingCreatureBase { public: int id; };
class CSoundPair { public: long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
static int g_strCtor = 0, g_strDtor = 0; static const char* g_lastText = (const char*)1; static long g_lastLen = 0;
struct CCharString_Retail {
    void* m_pData;
    CCharString_Retail(const char* text, long length = -1) : m_pData((void*)text) { ++g_strCtor; g_lastText = text; g_lastLen = length; }
    ~CCharString_Retail() { ++g_strDtor; }
};
static CThingCreatureBase* g_spOwner = 0; static int g_spLiveStrings = -1;
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString_Retail& name) { CSoundPair p; g_spOwner = owner; g_spLiveStrings = g_strCtor - g_strDtor; p.m_first = 11; p.m_second = 22; return p; }
static CThingCreatureBase* g_owner = 0; static long g_code = -1; static long g_first = -1, g_second = -1; static int g_baseCalls = 0, g_baseLiveStrings = -1;
class CCombatActionBase { public:
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s) { ++g_baseCalls; g_owner = owner; g_code = code; g_first = s.m_first; g_second = s.m_second; g_baseLiveStrings = g_strCtor - g_strDtor; }
};

class CCreatureAction_OnGenerate : public CCombatActionBase { public:
    virtual void v00() {}
    CCreatureAction_OnGenerate(CThingCreatureBase* owner) : CCombatActionBase(owner, 250, GetSoundPair(owner, static_cast<const char*>(0))) { }
};
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CCreatureAction_OnGenerate action(&owner);
    bool ok = g_baseCalls == 1 && g_owner == &owner && g_code == 250 && g_first == 11 && g_second == 22
        && g_spOwner == &owner && g_strCtor == 1 && g_strDtor == 1 && g_lastLen == -1 && (g_lastText == 0)
        && g_spLiveStrings == 1 && g_baseLiveStrings == 1;
    if (!ok) { std::printf("0074dbb0_TEST FAIL\n"); return 1; }
    std::printf("0074dbb0_TEST PASS\n"); return 0;
}
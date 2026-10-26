// Behaviour test for 0x00E81F60 CV_FisticuffsClubScript ctor: argument words, timer registration order,
// zeroed script-thing pointers and the five default-constructed region names.
#include <cstdio>
static long g_nextTimer = 100;
static int g_strings = 0;
class CGameScriptInterface { public:
    virtual long RegisterTimer() { return g_nextTimer++; }
};
static CGameScriptInterface g_iface;
static CGameScriptInterface* g_pGameScriptInterface = &g_iface;
class CTimer { public: long Index; CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { } };
class CScriptBase { public: virtual ~CScriptBase() { } unsigned char _body[0x3c]; CScriptBase() { } };
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game; void* Data;
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CBaseClass { public: virtual ~CBaseClass() { } CBaseClass() { } };
template<class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
class CScriptThing : public CBaseClass { public: CCountedPointer<CScriptThing> PImp; CScriptThing() : CBaseClass(), PImp() { } };
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CV_FisticuffsClubScript : CQuestScriptObject {
    long RelevantRegion;
    bool f[13];
    CTimer Fighter1Timer, Fighter2Timer;
    CScriptThing SFighterOne, SFighterTwo;
    long HitID, HitCount, AttackWarnings, FightCount;
    bool g[3];
    long AllowedHits;
    bool h[5];
    CScriptThing Village;
    bool PlayerLost;
    float CheerNumber, BooNumber;
    CCharString RegionNames[5];
    CV_FisticuffsClubScript(void* data, CGameScriptInterface* game) : CQuestScriptObject(data, game) { }
};
int main()
{
    int dataWord = 7;
    CV_FisticuffsClubScript s(&dataWord, &g_iface);
    if (s.Game != &g_iface || s.Data != &dataWord || s.Fighter1Timer.Index != 100 || s.Fighter2Timer.Index != 101
        || s.SFighterOne.PImp.Data || s.SFighterOne.PImp.Info || s.SFighterTwo.PImp.Info || s.Village.PImp.Data
        || g_strings != 5 || g_nextTimer != 102) {
        std::printf("00e81f60_TEST FAIL\n");
        return 1;
    }
    std::printf("00e81f60_TEST PASS\n");
    return 0;
}
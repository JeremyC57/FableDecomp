// Behaviour test for the CQ_RansomVictimScript ctor (0x00DE0320).
#include <cstdio>
#include <cstdlib>
static int g_baseObjects = 0, g_scriptBases = 0, g_strings = 0;
class CGameScriptInterface { public:
    long NextTimer;
    long RegisterTimer() { return NextTimer++; }
};
class CScriptDataBase { public: int Dummy; };
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
class CCPPointerInfo { public: long RefCount; };
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
class CBaseObject { public:
    void* IntelligentPointer;
    CBaseObject() : IntelligentPointer(0) { ++g_baseObjects; }
};
class CScriptGameResourceObjectBase : public CBaseObject { public:};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
};
class CScriptBase { public: CScriptBase() { ++g_scriptBases; } };
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;
    CScriptDataBase*      Data;
    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}
};
static bool Empty(const CScriptGameResourceObjectScriptedThingBase& h) { return h.PImp.Data == 0 && h.PImp.Info == 0 && h.IntelligentPointer == 0; }
static CGameScriptInterface g_global;
static CGameScriptInterface* g_pGameScriptInterface = &g_global;
class CTimer { public:
    long TimerIndex;
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) {}
};
struct CQ_RansomVictimScript : CQuestScriptBase {
    bool Flags[12]; long HitChief, HitEvilBrotherAndProtection; bool SomeoneDealingWithBeingHit; long KidnappersKilled;
    CCharString ActiveEnemyName;
    CTimer NPCCommentTimer;
    CScriptGameResourceObjectScriptedThingBase seh_Guard, seh_Father, seh_Victim, seh_Brother, seh_Ransommer;
    CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi);
};
CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi)
    : CQuestScriptBase(data, &gsi)
{
}
int main()
{
    CGameScriptInterface gsi; CScriptDataBase db; g_global.NextTimer = 40;
    CQ_RansomVictimScript* q = new CQ_RansomVictimScript(&db, gsi);
    int bad = q->GSI != &gsi || q->Data != &db || g_scriptBases != 1 || g_strings != 1 || g_baseObjects != 5
        || q->NPCCommentTimer.TimerIndex != 40 || g_global.NextTimer != 41
        || !Empty(q->seh_Guard) || !Empty(q->seh_Father) || !Empty(q->seh_Victim) || !Empty(q->seh_Brother) || !Empty(q->seh_Ransommer);
    if (bad) { std::printf("00de0320_TEST FAIL\n"); return 1; }
    std::printf("00de0320_TEST PASS\n");
    return 0;
}
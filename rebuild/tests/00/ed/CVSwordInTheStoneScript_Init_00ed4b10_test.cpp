// Behaviour test for 00ed4b10: body copied from src/00ed4b10.cpp (CV_SwordInTheStoneScript::Init); the game script interface
// (slot 89 SetTimer) and the counted-pointer destroy callback stubbed to record calls.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define GSI_V10(p) virtual void v##p##0() {} virtual void v##p##1() {} virtual void v##p##2() {} virtual void v##p##3() {} virtual void v##p##4() {} virtual void v##p##5() {} virtual void v##p##6() {} virtual void v##p##7() {} virtual void v##p##8() {} virtual void v##p##9() {}
static int g_deletes = 0; static void* g_lastDeleted = 0;
struct CGameScriptInterface {
    GSI_V10(0) GSI_V10(1) GSI_V10(2) GSI_V10(3) GSI_V10(4) GSI_V10(5) GSI_V10(6) GSI_V10(7)
    virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {}
    virtual long RegisterTimer() { return 0; }
    virtual void v088() {}
    virtual void SetTimer(long timer, long time) const { ++SetTimerCalls; LastTimer = timer; LastTime = time; }
    mutable int SetTimerCalls; mutable long LastTimer, LastTime;
    CGameScriptInterface() : SetTimerCalls(0), LastTimer(-1), LastTime(-1) {}
};
static CGameScriptInterface g_gsi;
CGameScriptInterface* g_pGameScriptInterface = &g_gsi;
struct CTimer {
    long Index;
    void SetTime(long time) { CGameScriptInterface* gsi = g_pGameScriptInterface; long index = Index; gsi->SetTimer(index, time); }
};
struct CScriptBase {
    virtual void v00() {}
    unsigned char _body[0x3c];
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;
    void*                 Data;
};
struct CCPPointerInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptThingImp;
static int g_destroys = 0; static void* g_destroyed = 0;
static void __fastcall DestroyImp(void* p) { ++g_destroys; g_destroyed = p; }
struct CCountedPointer_CScriptThing {
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    CCountedPointer_CScriptThing& operator=(const CCountedPointer_CScriptThing& other)
    {
        CScriptThingImp* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    ++g_deletes; g_lastDeleted = Info;   // stands in for `delete Info` (the test keeps the block)
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
struct CBaseClass {
    virtual void v00() {}
};
struct CScriptThing : CBaseClass {
    CCountedPointer_CScriptThing PImp;
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
struct CV_SwordInTheStoneScript : CQuestScriptObject {
#pragma pack(push, 1)
    struct SAttempt {
        long Count;
        bool Cutscene;
        bool CrowdPulling;
        void Reset() { Count = 0; Cutscene = false; CrowdPulling = false; }
    } Attempt;
#pragma pack(pop)
    bool         CrowdSwordPullActive;
    bool         HeroHasBeenToldSwordLegend;
    bool         HeroCongratulated;
    bool         CrowdDisperse;
    bool         HeroHasTakenSword;
    bool         SwordInTheStoneIntroComplete;
    long         HeroStrengthPhysique;
    long         HeroStrengthToughness;
    long         HeroStrengthHealth;
    bool         StatsInitialised;
    bool         AttackedByHero;
    CScriptThing CrowdMemberPullingCurrent;
    CScriptThing CrowdMemberPullingNull;
    CTimer       CrowdSwordTimer;
    void Init();
};
void CV_SwordInTheStoneScript::Init()
{
    Attempt.Reset();
    CrowdSwordTimer.SetTime(5);
    HeroHasBeenToldSwordLegend = false;
    HeroCongratulated = false;
    CrowdDisperse = false;
    HeroHasTakenSword = false;
    SwordInTheStoneIntroComplete = false;
    StatsInitialised = false;
    AttackedByHero = false;
    CrowdSwordPullActive = false;
    CrowdMemberPullingCurrent = CrowdMemberPullingNull;
}
int main()
{
    CHECK((char*)&((CV_SwordInTheStoneScript*)0)->CrowdSwordPullActive == (char*)0 + 0x4e);
    CHECK((char*)&((CV_SwordInTheStoneScript*)0)->CrowdMemberPullingCurrent == (char*)0 + 0x64);
    CHECK((char*)&((CV_SwordInTheStoneScript*)0)->CrowdSwordTimer == (char*)0 + 0x7c);
    CV_SwordInTheStoneScript s; memset(&s.Game, 0xff, sizeof(s) - 0x40);
    CCPPointerInfo oldInfo; oldInfo.m_references = 1; oldInfo.m_pfnDestroy = DestroyImp; oldInfo.m_pObject = (void*)0x1234;
    CCPPointerInfo nullInfo; nullInfo.m_references = 1; nullInfo.m_pfnDestroy = DestroyImp; nullInfo.m_pObject = (void*)0x5678;
    s.CrowdMemberPullingCurrent.PImp.Data = (CScriptThingImp*)0x11; s.CrowdMemberPullingCurrent.PImp.Info = &oldInfo;
    s.CrowdMemberPullingNull.PImp.Data = (CScriptThingImp*)0x22; s.CrowdMemberPullingNull.PImp.Info = &nullInfo;
    s.CrowdSwordTimer.Index = 42; s.HeroStrengthPhysique = 7; s.HeroStrengthToughness = 8; s.HeroStrengthHealth = 9;
    s.Init();
    CHECK(s.Attempt.Count == 0 && !s.Attempt.Cutscene && !s.Attempt.CrowdPulling);
    CHECK(g_gsi.SetTimerCalls == 1 && g_gsi.LastTimer == 42 && g_gsi.LastTime == 5);
    CHECK(!s.HeroHasBeenToldSwordLegend && !s.HeroCongratulated && !s.CrowdDisperse && !s.HeroHasTakenSword && !s.SwordInTheStoneIntroComplete);
    CHECK(!s.StatsInitialised && !s.AttackedByHero && !s.CrowdSwordPullActive);
    CHECK(s.HeroStrengthPhysique == 7 && s.HeroStrengthToughness == 8 && s.HeroStrengthHealth == 9);   // untouched
    CHECK(g_destroys == 1 && g_destroyed == (void*)0x1234 && g_deletes == 1 && g_lastDeleted == &oldInfo);  // the old puller released
    CHECK(s.CrowdMemberPullingCurrent.PImp.Data == (CScriptThingImp*)0x22 && s.CrowdMemberPullingCurrent.PImp.Info == &nullInfo);
    CHECK(nullInfo.m_references == 2);
    s.Init();                                                   // same handle again: no release, no add-ref
    CHECK(g_destroys == 1 && nullInfo.m_references == 2 && g_gsi.SetTimerCalls == 2);
    if (fails == 0) printf("00ed4b10_TEST PASS\n"); else printf("00ed4b10_TEST FAIL\n");
    return fails;
}
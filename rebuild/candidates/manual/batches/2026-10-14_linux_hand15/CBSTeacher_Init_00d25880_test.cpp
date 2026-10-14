#include <stdio.h>
#include <stddef.h>
static long g_thread = -1; static int g_calls;
struct CScriptThing { long m_id; long m_uid; long m_type; };
struct CV_HeroDollsScript { void InitCheckThread(long thread) { g_thread = thread; ++g_calls; } };
struct CQ_SunnyvaleMasterData { unsigned char _pad[0xbb4]; CScriptThing KidIntro; };
CQ_SunnyvaleMasterData* g_pMasterData;
struct CBS_Teacher { unsigned char _pad[0x14]; CV_HeroDollsScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData; CScriptThing KidIntro; void Init(); };
void CBS_Teacher::Init()
{
    ParentClass->InitCheckThread(0);
    KidIntro.m_id = g_pMasterData->KidIntro.m_id;
    KidIntro.m_uid = g_pMasterData->KidIntro.m_uid;
    KidIntro.m_type = g_pMasterData->KidIntro.m_type;
}
int main()
{
    CQ_SunnyvaleMasterData* md = new CQ_SunnyvaleMasterData; md->KidIntro.m_id = 1; md->KidIntro.m_uid = 2; md->KidIntro.m_type = 3; g_pMasterData = md;
    CV_HeroDollsScript parent; CBS_Teacher t; t.ParentClass = &parent; t.PMasterData = md; t.KidIntro.m_id = t.KidIntro.m_uid = t.KidIntro.m_type = 0;
    t.Init();
    bool ok = g_calls == 1 && g_thread == 0 && t.KidIntro.m_id == 1 && t.KidIntro.m_uid == 2 && t.KidIntro.m_type == 3 && offsetof(CBS_Teacher, KidIntro) == 0x1c;
    delete md;
    if (!ok) { printf("00d25880_TEST FAIL\n"); return 1; }
    printf("00d25880_TEST PASS\n"); return 0;
}

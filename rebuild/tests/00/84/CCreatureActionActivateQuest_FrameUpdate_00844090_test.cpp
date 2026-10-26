#include <cstdio>
#include <cstring>
class CCharString { public: char* m_pData; };
class CQuestManager { public: bool FindByName(const CCharString& name) { return std::strcmp(name.m_pData, "Q_ACTIVE") == 0; } };
CQuestManager g_qm; CQuestManager* g_pQuestManager = &g_qm;
static int g_finished = 0;
class CCreatureAction_ActivateQuest { public: virtual void FinishAction() { ++g_finished; } unsigned char _pad04[0xa8 - 4]; CCharString ScriptName; unsigned char LoadResources;
    void FrameUpdate() { if (!g_pQuestManager->FindByName(ScriptName)) FinishAction(); } };
int main()
{
    CCreatureAction_ActivateQuest a; a.ScriptName.m_pData = (char*)"Q_ACTIVE"; a.FrameUpdate();
    int f1 = g_finished; a.ScriptName.m_pData = (char*)"Q_GONE"; a.FrameUpdate();
    if (f1 != 0 || g_finished != 1 || (char*)&a.ScriptName - (char*)&a != 0xa8) { std::printf("00844090_TEST FAIL\n"); return 1; }
    std::printf("00844090_TEST PASS\n");
    return 0;
}
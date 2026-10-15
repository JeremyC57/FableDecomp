// Behaviour test for CQ_WizardBattleScript::EndMission() @ 0x00E1D640
#include <cstdio>

static char g_log[16];
static int  g_logCount = 0;
static void Log(char c) { if (g_logCount < 15) g_log[g_logCount++] = c; g_log[g_logCount] = 0; }

struct CGameScriptInterface_Calls {
    bool m_teleporting; bool m_questInfo; long m_experience;
    virtual void GiveHeroExperience(long amount) { m_experience = amount; Log('X'); }
    virtual void DisplayQuestInfo(bool display) { m_questInfo = display; Log('Q'); }
    virtual void SetTeleportingAsActive(bool active) { m_teleporting = active; Log('T'); }
};

struct CQ_WizardBattleMasterData { long m_experienceReward; };
static CQ_WizardBattleMasterData g_master = { 5000 };
static CQ_WizardBattleMasterData* g_pMasterData_0143e90c = &g_master;

struct CQ_WizardBattleScript {
    CGameScriptInterface_Calls* Game;
    bool MissionSucceeded;
    void CleanUp() { Log('C'); }
    void FinishMazeStage() { Log('F'); }
    void EndMission()
    {
        CleanUp();
        Game->SetTeleportingAsActive(true);
        Game->DisplayQuestInfo(false);
        FinishMazeStage();
        Game->GiveHeroExperience(g_pMasterData_0143e90c->m_experienceReward);
        MissionSucceeded = true;
    }
};

int main()
{
    CGameScriptInterface_Calls game; game.m_teleporting = false; game.m_questInfo = true; game.m_experience = 0;
    CQ_WizardBattleScript s; s.Game = &game; s.MissionSucceeded = false;
    s.EndMission();
    if (!game.m_teleporting || game.m_questInfo || game.m_experience != 5000 || !s.MissionSucceeded) { std::printf("00e1d640_TEST FAIL\n"); return 1; }
    if (g_log[0] != 'C' || g_log[1] != 'T' || g_log[2] != 'Q' || g_log[3] != 'F' || g_log[4] != 'X' || g_logCount != 5) { std::printf("00e1d640_TEST FAIL\n"); return 1; }
    std::printf("00e1d640_TEST PASS\n");
    return 0;
}

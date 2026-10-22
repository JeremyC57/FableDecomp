// behaviour test for CGameflowAssistanceScript::Tutorial_CombatMultiplier (0x00CEFCC0): new-high messages, nag after stalling, master ends the thread
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
struct CGameflowMasterData { bool StopAssistance; };
static int g_frame, g_mult[64], g_msgs, g_limit; static const char* g_msg[16]; static CGameflowMasterData* g_data;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; if (g_frame >= g_limit) g_data->StopAssistance = true; }
    long GetPlayerCreatureCombatMultiplier() const { return g_mult[g_frame]; }
    bool IsPlayerWieldingWeapon() const { return true; }
    void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString&, bool, bool) const { g_msg[g_msgs++] = text.Text; }
    bool IsHeroControlledByPlayer() const { return true; }
};
struct CScriptDef { long CombatMultFramesPerLevel; long CombatMultMasterLevel; };
static CScriptDef g_def = { 4, 16 };
static CScriptDef* g_pScriptDef = &g_def;
struct CGameflowAssistanceScript {
    CGameScriptInterface* GSI; CGameflowMasterData* PMasterData;
    long CombatMultHighestLevel, CombatMultFramesSinceChange;
    bool IsActiveThreadTerminating() { return false; }
    void Run()
    {
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            if (PMasterData->StopAssistance) break;
            if (GSI->IsPlayerWieldingWeapon() && GSI->IsHeroControlledByPlayer()) {
                if (IsActiveThreadTerminating()) return;
                long multiplier = GSI->GetPlayerCreatureCombatMultiplier();
                if (multiplier > CombatMultHighestLevel) {
                    if (IsActiveThreadTerminating()) return;
                    CombatMultHighestLevel = multiplier; CombatMultFramesSinceChange = 0;
                    if (multiplier > g_pScriptDef->CombatMultMasterLevel) {
                        if (IsActiveThreadTerminating()) return;
                        GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false);
                        return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (CombatMultHighestLevel == 15) GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false);
                    else if (CombatMultHighestLevel == 10) GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false);
                    else if (CombatMultHighestLevel == 5) GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ++CombatMultFramesSinceChange;
                    if (CombatMultFramesSinceChange > g_pScriptDef->CombatMultFramesPerLevel * CombatMultHighestLevel) {
                        if (IsActiveThreadTerminating()) return;
                        GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false);
                        CombatMultFramesSinceChange = 0;
                    }
                }
            }
            GSI->NewScriptFrame();
            if (!IsActiveThreadTerminating()) continue;
            return;
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    // multiplier 1 for frames 0..2, then 5 (progress) for 3..14, 10 at 15, 20 (master) at 20
    for (int i = 0; i < 64; ++i) g_mult[i] = i < 3 ? 1 : i < 15 ? 5 : i < 20 ? 10 : 20;
    CGameScriptInterface gsi; CGameflowMasterData data = { false }; g_data = &data; g_limit = 60;
    CGameflowAssistanceScript s; s.GSI = &gsi; s.PMasterData = &data; s.CombatMultHighestLevel = 0; s.CombatMultFramesSinceChange = 0;
    s.Run();
    // frame 0: no message for 1; frame 3: PROGRESS; frames 4..14 stall (allowance 4*5 = 20, no nag); 15: ADEPT; 20: MASTER ends
    if (g_msgs != 3 || std::strcmp(g_msg[0], "TEXT_QST_080_MULT_PROGRESS") || std::strcmp(g_msg[1], "TEXT_QST_080_MULT_ADEPT")
        || std::strcmp(g_msg[2], "TEXT_QST_080_MULT_MASTER") || g_frame != 20) { std::printf("00cefcc0_TEST FAIL\n"); return 1; }
    // a stall longer than allowance * level nags (and resets the counter); the stop flag ends the thread
    for (int i = 0; i < 64; ++i) g_mult[i] = 2;
    g_msgs = 0; g_frame = 0; g_limit = 15; g_def.CombatMultFramesPerLevel = 3; data.StopAssistance = false;
    s.CombatMultHighestLevel = 0; s.CombatMultFramesSinceChange = 0;
    s.Run();
    // frame 0 sets level 2; frames 1..7 stall: the 7th unimproved frame (> 3*2) nags; frames 8..14 nag again at frame 14
    if (g_msgs != 2 || std::strcmp(g_msg[0], "TEXT_QST_080_MULT_TRY") || std::strcmp(g_msg[1], "TEXT_QST_080_MULT_TRY")
        || g_frame != 15 || s.CombatMultFramesSinceChange != 0) { std::printf("00cefcc0_TEST FAIL\n"); return 1; }
    std::printf("00cefcc0_TEST PASS\n");
    return 0;
}

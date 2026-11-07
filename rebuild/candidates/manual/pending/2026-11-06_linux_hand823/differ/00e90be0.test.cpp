// Behaviour test for NScript::CV_GuildMasterScript::CGuildMasterGameFlow::Main (0x00E90BE0): while Q_EndGameFocalSites is active
// play the focal-site intro once, hand out the quest card and teleporter, then per frame teleport the hero standing on the residue
// and answer talks (guildmaster speech) and hits (ATTACKED); outside the quest answer hits and talks, where talk at the
// waiting-for-wizard-battle stage asks the take-card question (yes: card + journal taken, no: the NO line).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct GuildMaster {
    int Frames, TerminateAt, QuestFrom, QuestTo, TalkMask, HitMask, OnResidueMask, Answer;
    bool WaitingForWizardBattle;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    bool QuestActive() const { return Frames >= QuestFrom && Frames < QuestTo; }
    void Main()
    {
        NewScriptFrame(); if (T()) return;
        while (!T()) {
            if (QuestActive()) {
                Log("CS_FOCALSITEINTRO"); Log("card");
                while (QuestActive()) {
                    NewScriptFrame(); if (T()) return;
                    if (Bit(OnResidueMask)) Log("teleport");
                    if (Bit(TalkMask)) Log("SPEECH");
                    if (Bit(HitMask)) Log("ATTACKED");
                }
                if (T()) return;
            }
            if (Bit(HitMask)) Log("ATTACKED");
            if (Bit(TalkMask)) {
                if (WaitingForWizardBattle) {
                    Log("WAITING"); Log("QUESTION");
                    if (Answer == 1) { Log("YES"); Log("card_wizard"); } else Log("NO");
                } else Log("SPEECH");
            }
            NewScriptFrame();
        }
    }
};
int main()
{
    GuildMaster a = { 0, 12, 2, 6, (1 << 4) | (1 << 8), (1 << 5), (1 << 3), 1, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_FOCALSITEINTRO;card;teleport;SPEECH;ATTACKED;WAITING;QUESTION;YES;card_wizard;");
    g_log[0] = 0;
    GuildMaster b = { 0, 5, 99, 99, (1 << 2), 0, 0, 0, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "SPEECH;") && b.Frames == 5;
    std::printf(ok ? "00e90be0_TEST PASS\n" : "00e90be0_TEST FAIL\n");
    return ok ? 0 : 1;
}

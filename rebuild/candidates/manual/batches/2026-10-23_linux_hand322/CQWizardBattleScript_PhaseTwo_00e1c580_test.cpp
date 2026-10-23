#include <cstdio>
// behaviour test: CQ_WizardBattleScript::PhaseTwo -- while Maze lives, taunt on a hero hit (1 in 10), on the hero
// straying (1 in 200) or in battle (1 in 100), only when no conversation is running; stop when the mission fails
static int g_frame = 0, g_lines = 0, g_hitLines = 0, g_conversations = 0;
static unsigned int g_seed = 1;
static int Rand() { g_seed = g_seed * 1103515245u + 12345u; return (int)((g_seed >> 16) & 0x7fff); }
struct Script {
    bool MissionFailed; long MazeConversation; int mazeHealth; int convActiveUntil;
    bool IsActiveThreadTerminating() { return g_frame > 5000; }
    void Frame() { ++g_frame; if (g_frame == 3000) MissionFailed = true; }
    bool MazeAlive() { return mazeHealth > 0; }
    bool HitByHero() { return g_frame % 7 == 0; }
    bool HitBySpecial() { return g_frame % 11 == 0; }
    bool HitBySpecialFlood() { return g_frame % 22 == 0; }   // ability 14 does not count
    bool HeroFar() { return (g_frame / 100) % 2 == 1; }
    bool ConversationActive() { return g_frame < convActiveUntil; }
    void Say(bool hitLine) { MazeConversation = ++g_conversations; convActiveUntil = g_frame + 20; ++g_lines; if (hitLine) ++g_hitLines; }
    void PhaseTwo()
    {
        Frame();
        if (IsActiveThreadTerminating()) return;
        while (MazeAlive()) {
            Frame();
            if (IsActiveThreadTerminating()) return;
            bool hit = HitByHero() || (HitBySpecial() && !HitBySpecialFlood());
            if (hit && Rand() % 10 == 0 && !ConversationActive()) Say(true);
            if (HeroFar()) { if (Rand() % 200 == 0 && !ConversationActive()) Say(false); }
            else if (Rand() % 100 == 0 && !ConversationActive()) Say(false);
            if (MissionFailed) return;
            Frame();
            if (IsActiveThreadTerminating()) return;
        }
    }
};
int main()
{
    Script s; s.MissionFailed = false; s.MazeConversation = 0; s.mazeHealth = 1; s.convActiveUntil = 0;
    s.PhaseTwo();
    if (!s.MissionFailed || g_frame < 3000 || g_frame > 3002 || g_lines == 0 || g_hitLines == 0 || s.MazeConversation != g_conversations) {
        std::printf("00e1c580_TEST FAIL\n"); return 1;
    }
    std::printf("00e1c580_TEST PASS\n");
    return 0;
}

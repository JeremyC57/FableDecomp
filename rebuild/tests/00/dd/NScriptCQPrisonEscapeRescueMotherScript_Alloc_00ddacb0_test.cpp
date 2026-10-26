#include <cstdio>
// Behaviour test for 00ddacb0: CQ_PrisonEscapeRescueMotherScript::Alloc builds the script with its interface/data pair and a fresh RunningTimer.
struct Interface { long nextTimer; long RegisterTimer() { return nextTimer++; } };
static Interface g_interface;
class CTimer { public: long Index; CTimer() : Index(g_interface.RegisterTimer()) {} };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct Script : Base {
    long CurrentTask; long TasksDone[3]; long TargetNumber[3];
    bool flags[8]; long YearsPassedInCell, PrisonerConversationIndex, ConversationIndex; bool tail[3];
    CTimer RunningTimer;
    Script(void* data, Interface& game) : Base(data, game) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    g_interface.nextTimer = 11;
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    Script* t = Alloc(0, g_interface);
    int ok = s && t && s->Game == &g_interface && s->MasterScriptData == &token && t->MasterScriptData == 0
          && s->RunningTimer.Index == 11 && t->RunningTimer.Index == 12 && g_interface.nextTimer == 13;
    delete s; delete t;
    if (!ok) { std::printf("00ddacb0_TEST FAIL\n"); return 1; }
    std::printf("00ddacb0_TEST PASS\n");
    return 0;
}
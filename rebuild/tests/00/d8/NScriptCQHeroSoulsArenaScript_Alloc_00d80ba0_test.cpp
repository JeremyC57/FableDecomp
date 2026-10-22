#include <cstdio>
// Behaviour test for 00d80ba0: CQ_HeroSoulsArenaScript::Alloc builds the script with its interface/data pair, a fresh timer and empty rounds.
struct Interface { long nextTimer; long RegisterTimer() { return nextTimer++; } };
static Interface g_interface;
struct Script {
    Interface* Game;
    void* Data;
    long GuardChatTimer;
    void* RoundsStart; void* RoundsFinish; void* RoundsEnd;
    Script(void* data, Interface& game) : Game(&game), Data(data), GuardChatTimer(g_interface.RegisterTimer()), RoundsStart(0), RoundsFinish(0), RoundsEnd(0) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    g_interface.nextTimer = 5;
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    Script* t = Alloc(0, g_interface);
    int ok = s && t && s->Game == &g_interface && s->Data == &token && s->GuardChatTimer == 5 && t->GuardChatTimer == 6
          && s->RoundsStart == 0 && s->RoundsFinish == 0 && s->RoundsEnd == 0 && t->Data == 0;
    delete s; delete t;
    if (!ok) { std::printf("00d80ba0_TEST FAIL\n"); return 1; }
    std::printf("00d80ba0_TEST PASS\n");
    return 0;
}
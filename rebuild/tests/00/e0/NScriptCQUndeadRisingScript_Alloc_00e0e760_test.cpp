#include <cstdio>
// Behaviour test for 00e0e760: CQ_UndeadRisingScript::Alloc builds the script with its interface/data pair and two empty CScriptThing vectors.
struct Interface { int unused; };
static Interface g_interface;
struct Vec { void* start; void* finish; void* end; Vec() : start(0), finish(0), end(0) {} };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct Script : Base {
    unsigned int tokens, kills; bool f[4]; bool LightsActivated[8]; long quest_status;
    Vec activation_points; Vec boss_points;
    Script(void* data, Interface& game) : Base(data, game) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    int ok = s && s->Game == &g_interface && s->MasterScriptData == &token
          && s->activation_points.start == 0 && s->activation_points.finish == 0 && s->activation_points.end == 0
          && s->boss_points.start == 0 && s->boss_points.finish == 0 && s->boss_points.end == 0;
    delete s;
    if (!ok) { std::printf("00e0e760_TEST FAIL\n"); return 1; }
    std::printf("00e0e760_TEST PASS\n");
    return 0;
}
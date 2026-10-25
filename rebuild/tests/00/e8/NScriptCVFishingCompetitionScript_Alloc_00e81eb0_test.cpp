#include <cstdio>
// Behaviour test for 00e81eb0: CV_FishingCompetitionScript::Alloc builds the script with its interface/data pair, an empty Winner string and an empty SizeComment vector.
struct Interface { int unused; };
static Interface g_interface;
static int g_strings = 0;
struct CCharString { const char* PStringData; CCharString() : PStringData("") { ++g_strings; } };
struct Vec { long* start; long* finish; long* end; Vec() : start(0), finish(0), end(0) {} };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct Script : Base {
    bool f[9]; long AttackCounter, AttackDayCount, HitCount; bool DoneIntro, RodHintGiven; long PrizeNumber;
    CCharString Winner; float BiggestFish, BiggestCompetitorFish; Vec SizeComment;
    Script(void* data, Interface& game) : Base(data, game) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    int ok = s && s->Game == &g_interface && s->MasterScriptData == &token && g_strings == 1
          && s->Winner.PStringData[0] == 0 && s->SizeComment.start == 0 && s->SizeComment.finish == 0 && s->SizeComment.end == 0;
    delete s;
    if (!ok) { std::printf("00e81eb0_TEST FAIL\n"); return 1; }
    std::printf("00e81eb0_TEST PASS\n");
    return 0;
}
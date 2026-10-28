// Test: CV_FishingCompetitionScript ctor -- level-script base (Game, MasterScriptData), then an empty Winner string and an empty SizeComment vector.
#include <cstdio>
#include <cstring>
struct CStr { const char* s; CStr() : s("") { } };
struct CVec { int* first; int* last; int* end; CVec() : first(0), last(0), end(0) { } };
struct CLevel { void* Game; void* MasterScriptData; CLevel(void* data, void* game) : Game(game), MasterScriptData(data) { } };
struct CScript : CLevel { bool BountyHuntActive; CStr Winner; float BiggestFish; CVec SizeComment;
    CScript(void* data, void* game) : CLevel(data, game) { } };
int main()
{
    int data, game; CScript s(&data, &game);
    bool ok = s.Game == &game && s.MasterScriptData == &data && !std::strcmp(s.Winner.s, "") && s.SizeComment.first == 0 && s.SizeComment.end == 0;
    std::printf(ok ? "00e7e110_TEST PASS\n" : "00e7e110_TEST FAIL\n");
    return ok ? 0 : 1;
}
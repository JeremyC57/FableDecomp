// Behaviour test for 0x00E81F30 NScript::CV_FishingCompetitionScript::~CV_FishingCompetitionScript (compiler-generated):
// SizeComment's block freed, Winner destroyed, then ~CScriptBase.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
struct LongVector {
    long* Start; long* Finish; long* EndOfStorage;
    LongVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~LongVector() { FreeBlock(Start); }
};
struct CharString { void* PStringData; CharString() : PStringData(0) { } ~CharString() { Note(2); } };
struct ScriptBase { virtual ~ScriptBase() { Note(3); } unsigned char Body[0x3c]; };
struct LevelScriptBase : ScriptBase { void* Game; void* MasterScriptData; };
struct FishingCompetitionScript : LevelScriptBase {
    bool Flags[9];
    long AttackCounter, AttackDayCount, HitCount;
    bool DoneIntro, RodHintGiven;
    long PrizeNumber;
    CharString Winner;
    float BiggestFish, BiggestCompetitorFish;
    LongVector SizeComment;
};
int main()
{
    FishingCompetitionScript* script = new FishingCompetitionScript;
    script->SizeComment.Start = (long*)std::malloc(4 * sizeof(long));
    script->SizeComment.Finish = script->SizeComment.Start + 4;
    script->SizeComment.EndOfStorage = script->SizeComment.Finish;
    ScriptBase* base = script;
    delete base;
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3) { std::printf("00e81f30_TEST FAIL\n"); return 1; }
    g_n = 0;
    { FishingCompetitionScript empty; }
    if (g_n != 2 || g_log[0] != 2 || g_log[1] != 3) { std::printf("00e81f30_TEST FAIL\n"); return 1; }
    std::printf("00e81f30_TEST PASS\n");
    return 0;
}
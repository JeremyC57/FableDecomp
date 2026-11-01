// Behaviour test for NScript::CQ_ArenaScript::~CQ_ArenaScript (0x00CFAC40, compiler-generated): Whisper, GlobalCrowdTimer, CellsVillage, the Rounds vector (virtual element dtors forward, then storage), CrowdLoopTags[4][5] in reverse, then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[512];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct InfoModel { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void* data) { Log(*(const char*)data); }
struct ScriptThingModel {                        // CScriptThing: PImp counted pointer released in the dtor
    void* Data; InfoModel* Info;
    ScriptThingModel() : Data(0), Info(0) { }
    void Set(const char* tag) { Info = new InfoModel; Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = (void*)tag; Data = (void*)tag; }
    ~ScriptThingModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
static long g_lastDeregistered = -1;
struct TimerModel { long TimerIndex; char Tag; ~TimerModel() { g_lastDeregistered = TimerIndex; Log(Tag); } };
struct StringModel { char Tag; StringModel() : Tag('?') { } ~StringModel() { Log(Tag); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log('S'); } };
struct RoundDefModel { char Tag; virtual ~RoundDefModel() { Log(Tag); } };
struct RoundVectorModel {
    RoundDefModel* Start; RoundDefModel* Finish;
    ~RoundVectorModel() { for (RoundDefModel* p = Start; p != Finish; ++p) p->~RoundDefModel(); if (Start) { Log('V'); ::operator delete(Start); } }
};
struct ArenaScriptModel : ScriptBaseModel {
    StringModel      CrowdLoopTags[4][5];
    RoundVectorModel Rounds;
    ScriptThingModel CellsVillage;
    TimerModel       GlobalCrowdTimer;
    ScriptThingModel Whisper;
};
int main()
{
    ArenaScriptModel* s = new ArenaScriptModel;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 5; ++j) s->CrowdLoopTags[i][j].Tag = (char)('a' + i * 5 + j);
    RoundDefModel* rounds = (RoundDefModel*)::operator new(2 * sizeof(RoundDefModel));
    new (&rounds[0]) RoundDefModel; rounds[0].Tag = 'x';
    new (&rounds[1]) RoundDefModel; rounds[1].Tag = 'y';
    s->Rounds.Start = rounds; s->Rounds.Finish = rounds + 2;
    s->CellsVillage.Set("c");
    s->GlobalCrowdTimer.TimerIndex = 5; s->GlobalCrowdTimer.Tag = 'T';
    s->Whisper.Set("w");
    ScriptBaseModel* p = s;
    delete p;
    if (std::strcmp(g_log, "wTcxyVtsrqponmlkjihgfedcbaS") != 0 || g_lastDeregistered != 5) { std::printf("00cfac40_TEST FAIL\n"); return 1; }
    std::printf("00cfac40_TEST PASS\n");
    return 0;
}
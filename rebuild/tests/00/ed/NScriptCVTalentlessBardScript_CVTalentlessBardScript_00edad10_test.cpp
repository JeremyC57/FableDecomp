// Behaviour test for NScript::CV_TalentlessBardScript::~CV_TalentlessBardScript (0x00EDAD10, compiler-generated): RandomSpeechTimer, SongThingAvailable's storage, QuestInfo[11..0] (tag then name), SongItems (elements forward, then storage), BardWanderTime, stBard, then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <cstdlib>
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
struct ByteVectorModel { unsigned char* Start; ~ByteVectorModel() { if (Start) { Log('F'); std::free(Start); } } };
struct StringVectorModel {
    StringModel* Start; StringModel* Finish;
    ~StringVectorModel() { for (StringModel* p = Start; p != Finish; ++p) p->~StringModel(); if (Start) { Log('V'); std::free(Start); } }
};
struct QuestDataModel { StringModel name; StringModel tag; };
struct BardScriptModel : ScriptBaseModel {
    ScriptThingModel  stBard;
    TimerModel        BardWanderTime;
    StringVectorModel SongItems;
    QuestDataModel    QuestInfo[12];
    ByteVectorModel   SongThingAvailable;
    TimerModel        RandomSpeechTimer;
};
int main()
{
    BardScriptModel* s = new BardScriptModel;
    s->stBard.Set("s");
    s->BardWanderTime.TimerIndex = 1; s->BardWanderTime.Tag = 'W';
    s->SongItems.Start = (StringModel*)std::malloc(2 * sizeof(StringModel));
    s->SongItems.Start[0].Tag = 'a'; s->SongItems.Start[1].Tag = 'b'; s->SongItems.Finish = s->SongItems.Start + 2;
    for (int i = 0; i < 12; ++i) { s->QuestInfo[i].name.Tag = 'n'; s->QuestInfo[i].tag.Tag = 't'; }
    s->SongThingAvailable.Start = (unsigned char*)std::malloc(4);
    s->RandomSpeechTimer.TimerIndex = 7; s->RandomSpeechTimer.Tag = 'R';
    ScriptBaseModel* p = s;
    delete p;
    char expect[64] = "RF";
    for (int i = 0; i < 12; ++i) std::strcat(expect, "tn");
    std::strcat(expect, "abVWsS");
    if (std::strcmp(g_log, expect) != 0 || g_lastDeregistered != 1) { std::printf("00edad10_TEST FAIL\n"); return 1; }
    std::printf("00edad10_TEST PASS\n");
    return 0;
}
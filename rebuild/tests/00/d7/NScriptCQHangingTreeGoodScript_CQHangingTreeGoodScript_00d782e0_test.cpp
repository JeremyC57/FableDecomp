// Behaviour test for NScript::CQ_HangingTreeGoodScript::~CQ_HangingTreeGoodScript (0x00D782E0, compiler-generated): members
// destroyed in reverse declaration order (float vector, two string vectors, two script-thing vectors, two strings, four
// script things), then ~CScriptBase.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[512];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct InfoModel { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void* data) { Log(*(const char*)data); }
struct ScriptThingModel {
    void* Data; InfoModel* Info;
    ScriptThingModel() : Data(0), Info(0) { }
    ScriptThingModel(const ScriptThingModel& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void Set(const char* tag) { Info = new InfoModel; Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = (void*)tag; Data = (void*)tag; }
    virtual ~ScriptThingModel()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
};
struct StringModel { char Tag; StringModel() : Tag('?') { } StringModel(char t) : Tag(t) { } ~StringModel() { Log(Tag); } };
struct ScriptBaseModel { virtual ~ScriptBaseModel() { Log('S'); } };
struct HangingTreeGoodModel : ScriptBaseModel {
    long HTG_State;
    ScriptThingModel Leader, Prisoner, PartySpawnPoint, ActiveWayPoint;
    StringModel CurrentLevel, ReasonForFailing;
    std::vector<ScriptThingModel> CurrentEnemyVector, CurrentGuardVector;
    std::vector<StringModel> RegionsWhereSpawned, AreasSpawnedIn;
    std::vector<float> GuardsHealthVector;
};
int main()
{
    HangingTreeGoodModel* s = new HangingTreeGoodModel;
    s->Leader.Set("L"); s->Prisoner.Set("P"); s->PartySpawnPoint.Set("W"); s->ActiveWayPoint.Set("A");
    s->CurrentLevel.Tag = 'c'; s->ReasonForFailing.Tag = 'r';
    s->RegionsWhereSpawned.reserve(4); s->AreasSpawnedIn.reserve(4);
    s->RegionsWhereSpawned.push_back(StringModel('x'));
    s->AreasSpawnedIn.push_back(StringModel('y'));
    g_log[0] = 0;
    { ScriptThingModel e; e.Set("E"); s->CurrentEnemyVector.push_back(e); }
    { ScriptThingModel g; g.Set("G"); s->CurrentGuardVector.push_back(g); }
    s->GuardsHealthVector.push_back(1.0f);
    ScriptBaseModel* p = s;
    delete p;
    if (std::strcmp(g_log, "yxGErcAWPLS") != 0) { std::printf("00d782e0_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00d782e0_TEST PASS\n");
    return 0;
}
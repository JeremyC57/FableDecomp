// Behaviour test for NScript::CV_TravellingHeroesScript::~CV_TravellingHeroesScript (0x00EE40F0, compiler-generated): csargs map (erase the nodes when the count is non-zero and reset the header links, then free the header node), seh_Boy, seh_Girl, seh_Rival (counted implementation released, then ~CScriptGameResourceObjectBase), then ~CScriptBase.
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
struct NodeModel { bool Color; NodeModel* Parent; NodeModel* Left; NodeModel* Right; };
static int g_erased;
static void Erase(NodeModel* root) { if (root) { Erase(root->Left); Erase(root->Right); delete root; ++g_erased; } }
struct MapModel {
    NodeModel* Header; unsigned int NodeCount;
    ~MapModel()
    {
        if (NodeCount != 0) {
            Log('E'); Erase(Header->Parent);
            Header->Left = Header; Header->Parent = 0; Header->Right = Header; NodeCount = 0;
        }
        if (Header) { Log('H'); delete Header; }
    }
};
struct ResourceObjectBaseModel { virtual ~ResourceObjectBaseModel() { Log('o'); } };
struct ScriptedThingBaseModel : ResourceObjectBaseModel { ScriptThingModel PImp; };
struct TravellingHeroesScriptModel : ScriptBaseModel {
    ScriptedThingBaseModel seh_Rival;
    ScriptedThingBaseModel seh_Girl;
    ScriptedThingBaseModel seh_Boy;
    MapModel               csargs;
};
int main()
{
    TravellingHeroesScriptModel* s = new TravellingHeroesScriptModel;
    s->seh_Rival.PImp.Set("r"); s->seh_Girl.PImp.Set("g"); s->seh_Boy.PImp.Set("b");
    NodeModel* header = new NodeModel; NodeModel* root = new NodeModel; NodeModel* left = new NodeModel;
    left->Left = left->Right = 0; root->Left = left; root->Right = 0; root->Parent = header;
    header->Parent = root; header->Left = left; header->Right = root;
    s->csargs.Header = header; s->csargs.NodeCount = 2;
    ScriptBaseModel* p = s;
    delete p;
    if (std::strcmp(g_log, "EHbogoroS") != 0 || g_erased != 2) { std::printf("00ee40f0_TEST FAIL\n"); return 1; }
    std::printf("00ee40f0_TEST PASS\n");
    return 0;
}
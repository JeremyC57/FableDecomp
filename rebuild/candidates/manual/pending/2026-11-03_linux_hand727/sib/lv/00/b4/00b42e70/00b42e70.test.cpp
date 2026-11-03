// Behaviour test for 0x00B42E70 CEngineWorldMap::~CEngineWorldMap: CloseStaticMapFile, then DeleteMap, then the members in
// reverse order (ChangedAreas, StaticMapFile, StaticMapBankName, VisibleMapList, MapList), then ~CEngineComponent.
#include <cstdio>
#include <cstring>
static char g_seq[32]; static int g_n = 0;
static void Log(char c) { g_seq[g_n++] = c; g_seq[g_n] = 0; }
struct Array { void* start; ~Array() { if (start) Log(start == (void*)1 ? 'A' : 'M'); } };
struct List { ~List() { Log('L'); } };
class CWideString { public: ~CWideString() { Log('W'); } };
class CBankFileAsync { public: virtual ~CBankFileAsync() { Log('F'); } };
class CStaticMapBankFile : public CBankFileAsync { public: };
class CEngineComponent { public: virtual ~CEngineComponent() { Log('E'); } void** GlobalPointerAddress; };
class CEngineWorldMap : public CEngineComponent { public:
    virtual ~CEngineWorldMap() { CloseStaticMapFile(); DeleteMap(); }
    void CloseStaticMapFile() { Log('c'); }
    void DeleteMap() { Log('d'); }
    short* Map; Array MapList; List VisibleMapList; CWideString StaticMapBankName; CStaticMapBankFile StaticMapFile; Array ChangedAreas;
};
int main()
{
    CEngineWorldMap* m = new CEngineWorldMap;
    m->MapList.start = (void*)2; m->ChangedAreas.start = (void*)1;
    CEngineComponent* c = m;
    delete c;
    if (std::strcmp(g_seq, "cdAFWLME") != 0) { std::printf("00b42e70_TEST FAIL\n"); return 1; }
    std::printf("00b42e70_TEST PASS\n");
    return 0;
}
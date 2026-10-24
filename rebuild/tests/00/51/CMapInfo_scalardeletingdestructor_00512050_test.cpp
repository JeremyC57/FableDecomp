// Behaviour test for CMapInfo scalar deleting destructor: SerialiseScript, LevelScriptName, LevelName, PMap released, then freed (0x00512050).
#include <cstdio>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_dataDeleted = 0;
static void DeleteData(void* data) { (void)data; ++g_dataDeleted; g_log[g_n++] = 50; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
struct C2DBoxI { long TLX, TLY, BRX, BRY; }; class CMap;
struct CMapInfo { C2DBoxI Area; CCountedPointer<CMap> PMap; CCharString LevelName; CCharString LevelScriptName; unsigned long MapUID;
    bool Used, Placed, Activated, Loading, LoadedOnPlayerProximity; CCharString SerialiseScript; unsigned long long LevStamp, TngStamp; bool IsSea; };
int main()
{
    CMapInfo* m = new CMapInfo; m->LevelName.id = 1; m->LevelScriptName.id = 2; m->SerialiseScript.id = 3; m->PMap.Data = 0; m->PMap.Info = NewInfo(1);
    delete m;
    bool ok1 = g_n == 4 && g_log[0] == 3 && g_log[1] == 2 && g_log[2] == 1 && g_log[3] == 50 && g_dataDeleted == 1;
    if (!(ok1)) { std::printf("00512050_TEST FAIL\n"); return 1; }
    std::printf("00512050_TEST PASS\n");
    return 0;
}
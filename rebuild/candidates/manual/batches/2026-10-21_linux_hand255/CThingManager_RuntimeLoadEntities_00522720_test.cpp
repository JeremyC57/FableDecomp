#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingManager::RuntimeLoadEntities(long map, set<CCharString>* names, vector<CThing*>* things) -- unless the main game component says otherwise (virtual slot 9), load a map's runtime-persisted entities: the level-persistent entity script, the (non-empty) game-persistent entity script, then the game-persistent binary save data, flagging the manager as loading while it runs
// Leaf: ?RuntimeLoadEntities@CThingManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00522720, length 349 bytes (+3 int3), `ret 0Ch`.
//   Name and arguments from the catalog / engine_api (`CThingManager::RuntimeLoadEntities(long, set<CCharString>*,
//   vector<CThing*>*)`).
// Callees: g_pMainGameComponent (0x013B86A0) virtual slot 9 (+0x24), CThingManager::LoadEntitiesFromScript @ 0x00520D00,
//   CEntityRuntimePersistence::GetGamePersistentSaveData(long, CArray<unsigned char>&) @ 0x00522530,
//   CMemoryDataInputStream(void*, unsigned long) @ 0x00994930, its destructor @ 0x00993B10 (manifest
//   "~CChunkedFileChunk", propagated), CThingManager::LoadEntitiesBinary @ 0x00521240, free @ 0x00BFEA14.
// Layout (retail; Ego_r.pdb is 0x1c higher here and its STL vectors carry a trailing allocator): CThingManager
//   LoadingEntitiesFromSaveGame +0x9c, EntityRuntimePersistence +0xa0 {LevelPersistentEntityScripts +0,
//   GamePersistentEntityScripts +0xc, the dead-entity map +0x18, SaveFormatVersion +0x24}; CEntityScriptInfo is 0x18
//   bytes {Script +0, SaveData +4, UncompressedSize +0x10, Valid +0x14}.
#include <stdlib.h>
struct CThing;
struct CCharString;
struct CCharStringData { const char* m_pData; unsigned long m_length; };
struct CCharString {
    CCharStringData* PStringData;
    bool IsEmpty() const { return !PStringData || !PStringData->m_length; }
};
template<class T> struct set;
template<class T> struct vector;
struct CASuspendableProcess;

template<class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CArray()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T* GetData() { return size() ? _M_start : (T*)this; }   // retail: `sub; jne; lea ecx,[data]`
};

struct CEntityScriptInfo {
    CCharString             Script;                     // +0x00
    CArray<unsigned char>   SaveData;                   // +0x04
    long                    UncompressedSize;           // +0x10
    bool                    Valid;                      // +0x14
};

struct CEntityRuntimePersistence {
    CArray<CEntityScriptInfo> LevelPersistentEntityScripts;     // +0x00
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;      // +0x0c
    unsigned char             PersistedDeadEntities[0xc];      // +0x18
    long                      SaveFormatVersion;                // +0x24
    bool GetGamePersistentSaveData(long map, CArray<unsigned char>& data);   // 0x00522530
    const CEntityScriptInfo* GetLevelPersistentScriptInfo(long map) const
    {
        if ((unsigned long)map < LevelPersistentEntityScripts.size() && LevelPersistentEntityScripts._M_start[map].Valid)
            return &LevelPersistentEntityScripts._M_start[map];
        return 0;
    }
    const CEntityScriptInfo* GetGamePersistentScriptInfo(long map) const
    {
        if ((unsigned long)map < GamePersistentEntityScripts.size() && GamePersistentEntityScripts._M_start[map].Valid)
            return &GamePersistentEntityScripts._M_start[map];
        return 0;
    }
};

struct CMemoryDataInputStream {
    unsigned char _body[0x1c];
    CMemoryDataInputStream(void* data, unsigned long size);    // 0x00994930
    ~CMemoryDataInputStream();                                  // 0x00993B10
};

struct CMainGameComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool v09();                                 // slot 9 (+0x24): true skips the load (unnamed)
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CThingManager {
    unsigned char             _pad00[0x9c];
    bool                      LoadingEntitiesFromSaveGame;      // +0x9c
    unsigned char             _pad9d[3];
    CEntityRuntimePersistence EntityRuntimePersistence;         // +0xa0
    void LoadEntitiesFromScript(const CCharString& script, long version, long map, set<CCharString>* names,
                                CASuspendableProcess* process, long flags, vector<CThing*>* things);   // 0x00520D00
    void LoadEntitiesBinary(CMemoryDataInputStream& stream, long version, long map, set<CCharString>* names,
                            CASuspendableProcess* process, long flags, vector<CThing*>* things);       // 0x00521240
    void RuntimeLoadEntities(long map, set<CCharString>* names, vector<CThing*>* things);
};

void CThingManager::RuntimeLoadEntities(long map, set<CCharString>* names, vector<CThing*>* things)
{
    if (g_pMainGameComponent->v09())
        return;
    LoadingEntitiesFromSaveGame = true;
    const CEntityScriptInfo* levelInfo = EntityRuntimePersistence.GetLevelPersistentScriptInfo(map);
    if (levelInfo)
        LoadEntitiesFromScript(levelInfo->Script, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    const CEntityScriptInfo* gameInfo = EntityRuntimePersistence.GetGamePersistentScriptInfo(map);
    if (gameInfo && !gameInfo->Script.IsEmpty())
        LoadEntitiesFromScript(gameInfo->Script, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    CArray<unsigned char> data;
    if (EntityRuntimePersistence.GetGamePersistentSaveData(map, data) && !data.empty()) {
        CMemoryDataInputStream stream(data.GetData(), data.size());
        LoadEntitiesBinary(stream, EntityRuntimePersistence.SaveFormatVersion, map, names, 0, 0, things);
    }
    LoadingEntitiesFromSaveGame = false;
}

void CMainGameComponent::v00() {}
void CMainGameComponent::v01() {}
void CMainGameComponent::v02() {}
void CMainGameComponent::v03() {}
void CMainGameComponent::v04() {}
void CMainGameComponent::v05() {}
void CMainGameComponent::v06() {}
void CMainGameComponent::v07() {}
void CMainGameComponent::v08() {}
static bool g_skip = false;
bool CMainGameComponent::v09() { return g_skip; }
static CMainGameComponent g_mgc; CMainGameComponent* g_pMainGameComponent = &g_mgc;
static char g_log[256]; static int g_n = 0;
static void Log(const char* s) { std::strcat(g_log, s); }
void CThingManager::LoadEntitiesFromScript(const CCharString& script, long version, long map, set<CCharString>* names, CASuspendableProcess* p, long f, vector<CThing*>* things)
{ char b[64]; std::sprintf(b, "S(%s,%ld,%ld,%d)", script.PStringData->m_pData, version, map, (int)(p == 0 && f == 0 && names == (void*)1 && things == (void*)2)); Log(b); if (!LoadingEntitiesFromSaveGame) Log("!"); }
static int g_streamLive = 0; static unsigned long g_streamSize = 0; static void* g_streamData = 0;
CMemoryDataInputStream::CMemoryDataInputStream(void* d, unsigned long s) { ++g_streamLive; g_streamSize = s; g_streamData = d; }
CMemoryDataInputStream::~CMemoryDataInputStream() { --g_streamLive; }
void CThingManager::LoadEntitiesBinary(CMemoryDataInputStream& st, long version, long map, set<CCharString>*, CASuspendableProcess*, long, vector<CThing*>*)
{ char b[64]; std::sprintf(b, "B(%lu,%ld,%ld,%d)", g_streamSize, version, map, g_streamLive); Log(b); }
static int g_saveBytes = 0;
bool CEntityRuntimePersistence::GetGamePersistentSaveData(long map, CArray<unsigned char>& data)
{
    if (g_saveBytes < 0) return false;
    if (g_saveBytes) { data._M_start = (unsigned char*)malloc(g_saveBytes); data._M_finish = data._M_start + g_saveBytes; data._M_end_of_storage = data._M_finish; }
    return true;
}
#define CHECK(c) if (!(c)) { std::printf("00522720_TEST FAIL line %d: %s\n", __LINE__, g_log); return 1; }

int main()
{
    static CCharStringData lvl = { "lvl", 3 }, gam = { "gam", 3 }, empty = { "", 0 };
    static CEntityScriptInfo level[3], game[3];
    level[1].Script.PStringData = &lvl; level[1].Valid = true;
    game[1].Script.PStringData = &gam; game[1].Valid = true;
    level[2].Script.PStringData = &lvl; level[2].Valid = false;
    game[2].Script.PStringData = &empty; game[2].Valid = true;
    static CThingManager mgr;
    mgr.EntityRuntimePersistence.LevelPersistentEntityScripts._M_start = level; mgr.EntityRuntimePersistence.LevelPersistentEntityScripts._M_finish = level + 3;
    mgr.EntityRuntimePersistence.GamePersistentEntityScripts._M_start = game; mgr.EntityRuntimePersistence.GamePersistentEntityScripts._M_finish = game + 2;
    mgr.EntityRuntimePersistence.SaveFormatVersion = 7;
    set<CCharString>* names = (set<CCharString>*)1; vector<CThing*>* things = (vector<CThing*>*)2;
    g_saveBytes = 5;
    mgr.RuntimeLoadEntities(1, names, things);
    CHECK(std::strcmp(g_log, "S(lvl,7,1,1)S(gam,7,1,1)B(5,7,1,1)") == 0 && !mgr.LoadingEntitiesFromSaveGame && g_streamLive == 0);
    g_log[0] = 0; g_saveBytes = 0;
    mgr.RuntimeLoadEntities(2, names, things);                  // invalid level entry, game array too short, no data
    CHECK(g_log[0] == 0);
    game[2].Valid = true; mgr.EntityRuntimePersistence.GamePersistentEntityScripts._M_finish = game + 3;
    mgr.RuntimeLoadEntities(2, names, things);                  // empty game script is skipped
    CHECK(g_log[0] == 0);
    g_saveBytes = -1; mgr.RuntimeLoadEntities(5, names, things);
    CHECK(g_log[0] == 0);
    g_skip = true; g_saveBytes = 3; mgr.RuntimeLoadEntities(1, names, things);
    CHECK(g_log[0] == 0);
    g_skip = false; mgr.RuntimeLoadEntities(0, names, things);  // only the binary data
    CHECK(std::strcmp(g_log, "B(3,7,0,1)") == 0);
    mgr.EntityRuntimePersistence.LevelPersistentEntityScripts._M_start = 0;   // static arrays: not ours to free
    mgr.EntityRuntimePersistence.GamePersistentEntityScripts._M_start = 0;
    std::printf("00522720_TEST PASS\n");
    return 0;
}

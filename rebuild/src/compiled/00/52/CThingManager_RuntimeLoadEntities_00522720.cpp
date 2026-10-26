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
class CThing;
class CCharString;
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    bool IsEmpty() const { return !PStringData || !PStringData->m_length; }
};
template<class T> struct set;
template<class T> struct vector;
class CASuspendableProcess;

template<class T> class CArray { public:
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

class CMemoryDataInputStream { public:
    unsigned char _body[0x1c];
    CMemoryDataInputStream(void* data, unsigned long size);    // 0x00994930
    ~CMemoryDataInputStream();                                  // 0x00993B10
};

class CMainGameComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool v09();                                 // slot 9 (+0x24): true skips the load (unnamed)
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CThingManager { public:
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
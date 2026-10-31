// CObject::Load(CASuspendableProcess* process) -- load every region level file: topology, objects, nav maps, then activate them and mark the region loaded
// Grade: DIFFER(1345v1329) -- loop register allocation: retail keeps the element offset in ebp and the index in a stack slot (loop 1), mine swaps them; loop 2/3 base+index folding differs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C2170, length 1344 bytes (thiscall, ret 4).
// Callees: GFGetTime 0x009E1BC0 (result discarded), CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 /
// ~CCharString 0x0099EAE0, __fastcall NProgressDisplay::DisplayProgress 0x009E9F40, CWorldMap virtual slots 6/7/22,
// CThingMapManager::AddGameMap 0x00638310, CThingManager::RuntimeLoadEntities 0x00522720 / LoadFromFile 0x00521AE0 /
// ActivateThings 0x0051E2F0, set<CCharString> copy ctor 0x00514400, CWorldMap::CreateNavMap 0x00500230 /
// ActivateNavMap 0x0050AF10 / 0x004FD020 (post-load initialise; manifest label is propagated) / ActivateTopology
// 0x004FCBB0 / SetRegionAsLoaded 0x004FC8A0, CEditWorldMap::SetMapAsChanged 0x004FCFE0, engine singleton 0x013B86A0
// slot 9, CMessageEventManager::AddMessage 0x00687540, free 0x00BFEA14.
// Layout (descriptive): +0x00 set<CCharString> sections, +0x0c world, +0x10 vector of 0x1c-byte level-file entries,
// +0x1c region id.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
double __fastcall GFGetTime();                             // 0x009E1BC0
struct CAFile;
struct CASuspendableProcess;
struct CThing;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
struct CCountedData { long Count; };
template<class T> struct CCountedPointer {
    T*            Object;
    CCountedData* Data;
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Data(other.Data) { if (Data) ++Data->Count; }
};
struct CSectionSet {
    void* Head;
    unsigned long Count;
    unsigned long Compare;
    CSectionSet(const CSectionSet&);                        // 0x00514400
};
struct CThingVector {
    CThing** Start;
    CThing** Finish;
    CThing** EndOfStorage;
    CThingVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CThingVector() { if (Start) free(Start); }
};
struct CThingMapManager { void AddGameMap(long level); };  // 0x00638310
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void LoadTopology(long level, CCountedPointer<CAFile> file, CASuspendableProcess* process);   // slot 6
    virtual void PostLoadTopology(long level);                                                        // slot 7
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void SetRegion(long region);                                                              // slot 22
    unsigned char _pad04[0x0c];
    CThingMapManager* ThingMapManager;                     // +0x10
    void CreateNavMap(long level, CSectionSet* sections, CAFile* file, CASuspendableProcess* process);   // 0x00500230
    void ActivateNavMap(long level);                       // 0x0050AF10
    void PostLoadInitialise(long level);                   // 0x004FD020
    void ActivateTopology(long level);                     // 0x004FCBB0
    void SetMapAsChanged(long level, bool changed);        // 0x004FCFE0
    void SetRegionAsLoaded(long region, const CCharString& name);   // 0x004FC8A0
};
struct CThingManager {
    void RuntimeLoadEntities(long level, CSectionSet* sections, CThingVector* things);   // 0x00522720
    void LoadFromFile(CCountedPointer<CAFile> file, long level, CSectionSet sections, CThingVector* things,
                      CASuspendableProcess* process, int mode);                         // 0x00521AE0
    void ActivateThings(CThingVector* things);                                          // 0x0051E2F0
};
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long a, const C3DVector* pos, CThing* t1, CThing* t2, float f1, float f2, long v);   // 0x00687540
};
struct CGameData {
    unsigned char _pad00[0x60];
    CMessageEventManager* MessageEventManager;             // +0x60
    unsigned char _pad64[0x20];
    CCharString RegionName;                                // +0x84
};
struct CWorld {
    unsigned char _pad00[0x18];
    CThingManager* ThingManager;                           // +0x18
    CWorldMap* WorldMap;                                   // +0x1c
    CGameData* GameData;                                   // +0x20
};
struct CEngineCalls {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual bool IsLoadingSuppressed();                    // slot 9
};
extern CEngineCalls* g_pEngine;                            // 0x013B86A0

struct CLevelFileEntry {                                   // 0x1c bytes
    long Level;                                            // +0x00
    CCountedPointer<CAFile> TopologyFile;                  // +0x04
    CAFile* NavFile;                                       // +0x0c
    long _10;
    CCountedPointer<CAFile> ThingsFile;                    // +0x14
};
struct CLevelFileVector {
    CLevelFileEntry* Start;
    CLevelFileEntry* Finish;
    CLevelFileEntry* EndOfStorage;
    unsigned long size() const { return Finish - Start; }
    CLevelFileEntry& operator[](unsigned long i) { return Start[i]; }
};

struct CObject {
    CSectionSet Sections;                                  // +0x00
    CWorld* World;                                         // +0x0c
    CLevelFileVector LevelFiles;                           // +0x10
    long Region;                                           // +0x1c
    void Load(CASuspendableProcess* process);
};

void CObject::Load(CASuspendableProcess* process)
{
    GFGetTime();
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        long level = LevelFiles[i].Level;
        if (LevelFiles[i].TopologyFile.Object) {
            CCountedPointer<CAFile>& file = LevelFiles[i].TopologyFile;
            NProgressDisplay::DisplayProgress("Loading topology", -1.0f, false, false);
            World->WorldMap->LoadTopology(level, file, process);
            World->WorldMap->ThingMapManager->AddGameMap(level);
            NProgressDisplay::DisplayProgress("Post load topology", -1.0f, false, false);
            World->WorldMap->PostLoadTopology(level);
            NProgressDisplay::DisplayProgress("Post load topology end", -1.0f, false, false);
        }
    }
    CThingVector things;
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        CCountedPointer<CAFile>& file = LevelFiles[i].ThingsFile;
        if (file.Object) {
            NProgressDisplay::DisplayProgress("Loading objects", -1.0f, false, false);
            long level = LevelFiles[i].Level;
            World->ThingManager->RuntimeLoadEntities(level, &Sections, &things);
            World->ThingManager->LoadFromFile(file, level, Sections, &things, process, 3);
        }
    }
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        if (LevelFiles[i].NavFile)
            World->WorldMap->CreateNavMap(LevelFiles[i].Level, &Sections, LevelFiles[i].NavFile, process);
    }
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        if (LevelFiles[i].NavFile)
            World->WorldMap->ActivateNavMap(LevelFiles[i].Level);
    }
    World->ThingManager->ActivateThings(&things);
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        NProgressDisplay::DisplayProgress("Region Level Files: Post Load Initialise", -1.0f, false, false);
        World->WorldMap->PostLoadInitialise(LevelFiles[i].Level);
    }
    for (unsigned long i = 0; i < LevelFiles.size(); ++i) {
        NProgressDisplay::DisplayProgress("Region Level Files: Activate Topology", -1.0f, false, false);
        long level = LevelFiles[i].Level;
        if (LevelFiles[i].TopologyFile.Object)
            World->WorldMap->ActivateTopology(level);
        ((CWorldMap*)World->WorldMap)->SetMapAsChanged(level, false);
        if (!g_pEngine->IsLoadingSuppressed()) {
            C3DVector zero;
            zero.X = 0.0f; zero.Y = 0.0f; zero.Z = 0.0f;
            World->GameData->MessageEventManager->AddMessage(0x24, 0x32, &zero, 0, 0, 0.0f, 0.0f, level);
        }
    }
    if (Region > 0) {
        if (!g_pEngine->IsLoadingSuppressed()) {
            C3DVector zero;
            zero.X = 0.0f; zero.Y = 0.0f; zero.Z = 0.0f;
            World->GameData->MessageEventManager->AddMessage(0x26, 0x32, &zero, 0, 0, 0.0f, 0.0f, Region);
        }
        World->WorldMap->SetRegion(Region);
        CCharString name(World->GameData->RegionName);
        World->WorldMap->SetRegionAsLoaded(Region, name);
    }
    GFGetTime();
}

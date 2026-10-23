// Grade: DIFFER(625v625) -- only residue: SetWorld call scratch pair swapped (retail mov eax,[ecx]; lea edx,[esi+4]; push edx; ours edx/eax)
// CWorld::PostInit(CDisplayEngine& engine) -- second-stage world set-up with loading-screen progress: create the atmos processor, the game script interface, the script info manager (and initialise it), the global quest manager, hand the world to the 3D engine, create the music manager, then refresh the sound system's ambient/music/voice categories
// Leaf: ?PostInit@CWorld@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A6550, 623 bytes (+ int3 padding), `ret 4` (virtual; FableWin.pdb
//   ?PostInit@CWorld@@UAEXAAVCDisplayEngine@@@Z).
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0, LoadingStage 0x009D8240 and DisplayProgress 0x009E9F40 (both __fastcall),
//   operator new 0x00BFEA1A; ctors CAtmosProcess 0x006B1960 (0x20 bytes), CGameScriptInterface 0x006E7740 (0x50),
//   CScriptInfoManager 0x00CB5C70 (0x20) + Init 0x00CB5D80, CQuestManager 0x004B4590 (0xb4), CMusicManager 0x006BE300 (0x40);
//   CCountedPointer<T>::Reset(T*) out of line per type: CAtmosProcess 0x004AB300, CScriptInfoManager 0x004AB370,
//   CQuestManager 0x004A9A10 (the global counted pointer @ 0x013B89FC), CMusicManager 0x004AE650; the display engine's 3D
//   engine (+0x2c) virtual slot 7 (SetWorld); g_pSoundSystem 0x013B8394 slot 46 RefreshCategory(long, float, float).
//   Literals "Init Atmos" 0x012390C0, "Init Atmos Processor" 0x012390A8, "Init Scripts" 0x01239098, "Engine Set World"
//   0x01239084, "Init Music Manager" 0x01239070. (Catalog labels on the ctors/Reset bodies are propagated and wrong.)
// Layout: the retail CWorld of the landed ctor 0x004A67D0 (Component +8, PlayerManager +0xc, DefinitionManager +0x10,
//   PAtmosProcessor +0x24, PGameScriptInterface +0x38, PScriptInfoManager +0x58, PMusicManager +0x6c; CIDrawWorld base +4).
// Shape: each progress string is an implicit literal temporary in its own call statement; the script interface goes through
//   an inline scoped_ptr reset (old pointer deleted through the virtual scalar-deleting dtor).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall LoadingStage(const CCharString& stage, long level);                         // 0x009D8240
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

inline void* operator new(unsigned int, void* where) { return where; }
struct CMainGameComponent;
struct CPlayerManager;
struct CGameDefinitionManager;
struct CCPPointerInfo;
class CWorld;
class CDisplayEngine;

struct CIDrawWorld {
    virtual void DrawWorld() = 0;
};
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
};

struct CAtmosProcess { unsigned char _body[0x20]; CAtmosProcess(); /* 0x006B1960 */ };
struct CScriptInfoManager { unsigned char _body[0x20]; CScriptInfoManager(); /* 0x00CB5C70 */ void Init(); /* 0x00CB5D80 */ };
struct CMusicManager { unsigned char _body[0x40]; CMusicManager(); /* 0x006BE300 */ };
struct CGameScriptInterface {
    virtual ~CGameScriptInterface();
    unsigned char _body[0x50 - 4];
    CGameScriptInterface(CMainGameComponent* component, CGameDefinitionManager* definitions, CWorld* world,
                         CPlayerManager* players, CDisplayEngine& engine);   // 0x006E7740
};
struct CQuestManager {
    unsigned char _body[0xb4];
    CQuestManager(CWorld* world, CScriptInfoManager* scriptInfo, CGameScriptInterface* gameScript);   // 0x004B4590
};

template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    void Reset(T* p);                                  // out of line per T
};
template <class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
};
extern CCountedPointer<CQuestManager> QuestManager;    // 0x013B89FC

struct C3DEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual void SetWorld(CIDrawWorld* world);         // slot 7 (+0x1c)
};
class CDisplayEngine {
public:
    unsigned char _pad[0x2c];
    C3DEngine* Engine;                                 // +0x2c
};
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;                   // 0x013B8394

class CWorld : public CBaseClassNonCopyable, public CIDrawWorld {
public:
    CMainGameComponent* Component;                     // +0x08
    CPlayerManager* PlayerManager;                     // +0x0c
    CGameDefinitionManager* DefinitionManager;         // +0x10
    void* PWorldMap;                                   // +0x14
    void* PEnvironment;                                // +0x18
    void* PGameTimeManager;                            // +0x1c
    void* PThingSearchTools;                           // +0x20
    CCountedPointer<CAtmosProcess> PAtmosProcessor;    // +0x24
    void* PGameCamera;                                 // +0x2c
    void* PGameCameraManager;                          // +0x30
    void* PCurrentGameCamera;                          // +0x34
    scoped_ptr<CGameScriptInterface> PGameScriptInterface;   // +0x38
    void* PMainMeshBank[2];                            // +0x3c
    void* PAnimationManager;                           // +0x44
    void* PNavigationManager;                          // +0x48
    void* PThingCombatManager;                         // +0x4c
    void* PThingManager;                               // +0x50
    void* PFactionManager;                             // +0x54
    CCountedPointer<CScriptInfoManager> PScriptInfoManager;  // +0x58
    void* PMessageEventManager[2];                     // +0x60
    void* PBulletTimeManager;                          // +0x68
    CCountedPointer<CMusicManager> PMusicManager;      // +0x6c
    virtual void DrawWorld();
    virtual void PostInit(CDisplayEngine& engine);
};

void CWorld::PostInit(CDisplayEngine& engine)
{
    LoadingStage("Init Atmos", 0);
    DisplayProgress("Init Atmos Processor", false, -1.0f, false);
    PAtmosProcessor.Reset(new CAtmosProcess);
    LoadingStage("Init Scripts", 0);
    DisplayProgress("Init Scripts", false, -1.0f, false);
    PGameScriptInterface.reset(new CGameScriptInterface(Component, DefinitionManager, this, PlayerManager, engine));
    PScriptInfoManager.Reset(new CScriptInfoManager);
    PScriptInfoManager.Data->Init();
    {
        void* memory = operator new(sizeof(CQuestManager));
        CQuestManager* questManager;
        if (memory) {
            CScriptInfoManager* scriptInfo = PScriptInfoManager.Data;
            questManager = new (memory) CQuestManager(this, scriptInfo, PGameScriptInterface.ptr);
        } else
            questManager = 0;
        QuestManager.Reset(questManager);
    }
    LoadingStage("Engine Set World", 0);
    DisplayProgress("Engine Set World", false, -1.0f, false);
    { CIDrawWorld* drawWorld = this; engine.Engine->SetWorld(drawWorld); }
    LoadingStage("Init Music Manager", 0);
    DisplayProgress("Init Music Manager", false, -1.0f, false);
    PMusicManager.Reset(new CMusicManager);
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0, 0);
        g_pSoundSystem->RefreshCategory(0x40, 0, 0);
        g_pSoundSystem->RefreshCategory(0x80, 0, 0);
    }
}

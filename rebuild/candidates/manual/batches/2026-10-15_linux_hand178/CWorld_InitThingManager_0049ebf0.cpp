// CWorld::InitThingManager() -- create the world's thing manager, replace (delete) any previous one and make it current
// Retail: Fable.exe @ 0x0049EBF0, 100 bytes. PDB ?InitThingManager@CWorld@@IAE_NXZ (protected)
// Callees: operator new 0x00BFEA1A (0xE8 bytes), CThingManager::CThingManager(const CMainGameComponent&,
// const CGameDefinitionManager&, CWorld&, CPlayerManager&, CNavigatorManager&, unsigned long* seed, unsigned long,
// unsigned long, unsigned long) 0x00523540 (PDB ??0CThingManager@@QAE@ABVCMainGameComponent@@ABVCGameDefinitionManager@@AAVCWorld@@AAVCPlayerManager@@AAVCNavigatorManager@@PAKKKK@Z),
// the old manager's virtual deleting destructor (slot 0, flag 1), the global __fastcall SetCurrentThingManager
// 0x004C74F0.
// CWorld (engine/CWorld.h offsets agree here): Component +0x08, PlayerManager +0x0C, DefinitionManager +0x10,
// PNavigationManager +0x48, PThingManager (scoped_ptr) +0x50; CMainGameComponent WorldSeed +0x1613C (PDB = retail).
// Shape: scoped_ptr::reset -- old pointer read, new one stored, then `delete old` (null test + virtual delete).
struct CGameDefinitionManager;
struct CPlayerManager;
struct CNavigatorManager;
struct CWorld;

struct CMainGameComponent {
    unsigned char _pad[0x1613c];
    unsigned long WorldSeed;                  // +0x1613C
};

struct CThingManager {
    CThingManager(const CMainGameComponent& component, const CGameDefinitionManager& definitions, CWorld& world,
                  CPlayerManager& players, CNavigatorManager& navigation, unsigned long* seed,
                  unsigned long a, unsigned long b, unsigned long c);   // 0x00523540
    virtual ~CThingManager();
    unsigned char _body[0xe4];                // 0xE8 bytes in all
};

void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0

template <class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
};

struct CWorld {
    void*                   __vftable;            // +0x00
    unsigned char           _base_0x04[0x4];      // +0x04
    CMainGameComponent*     Component;            // +0x08
    CPlayerManager*         PlayerManager;        // +0x0C
    CGameDefinitionManager* DefinitionManager;    // +0x10
    unsigned char           _pad14[0x34];
    scoped_ptr<CNavigatorManager> PNavigationManager;   // +0x48
    unsigned char           _pad4c[0x4];
    scoped_ptr<CThingManager> PThingManager;      // +0x50
    bool InitThingManager();
};

bool CWorld::InitThingManager()
{
    PThingManager.reset(new CThingManager(*Component, *DefinitionManager, *this, *PlayerManager, *PNavigationManager,
                                          &Component->WorldSeed, 0x100000, 0x1000, 0x800));
    SetCurrentThingManager(PThingManager.get());
    return true;
}

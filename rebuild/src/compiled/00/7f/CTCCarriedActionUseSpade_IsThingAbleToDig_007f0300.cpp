// CTCCarriedActionUseSpade::IsThingAbleToDig(const CThingCreatureBase& creature) -- a creature can dig only on bare ground: not standing on a thing, and the strongest engine theme under it has a material def flagged IsDiggable
// Leaf: ?IsThingAbleToDig@CTCCarriedActionUseSpade@@SI_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F0300, 199 bytes (+ int3 padding). Ego_r.pdb / FableWin.pdb
//   `?IsThingAbleToDig@CTCCarriedActionUseSpade@@SI_NABVCThingCreatureBase@@@Z` (public static `__fastcall`); callers
//   CTCCarriedActionUseSpade 0x007F03D0 and CContextSensitiveItemDef::IsValidOverride 0x00850A80.
// Callees: the creature's virtual GetThingStandingOn (slot 39, +0x9c: CThing returns 0, CThingGameObject its
//   PThingStandingOn), CThing::GetPosition 0x004C73D0, CWorld::PeekWorldMap (slot 13, +0x34),
//   CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long*) 0x0063ADF0, CDefinitionManager::GetDef<CEngineThemeDef>
//   0x0063F3A0 and GetDef<CMaterialDef> 0x0066C340 (manifest names both "<CThingPhysicalSwitchDef>": byte twins), the def's
//   virtual slot 1 (destroy).
// Globals: g_pMainGameComponent 0x013B86A0 (PWorld +0x24); CWorld DefinitionManager +0x10 (Ego_r.pdb).
// Layout: retail CEngineThemeDef MaterialDef +0x8c and CMaterialDef IsDiggable +0x4d (both 4 below Ego_r.pdb, as the landed
//   CTCStealth::UpdateViewability 0x00747480 finds for the theme def); def objects count references at +4.
// Shape: the whole test inside `if (!creature.GetThingStandingOn())` with one `return false` last; the world is a named
//   local (esi) and the def manager comes through an inline `GetDefinitionManager()` getter (an inline CALL as the object
//   expression is evaluated before the arguments -- edi, saved with ebp -- where the raw member is loaded after them; the
//   second GetDef re-reads it); the two CDefPointer locals
//   share one frame slot (the first enregistered in edi after its GetDef), which needs same-TU `auto_inline(off)` GetDef
//   stand-ins so VC7.1 sees the out pointers do not escape (GOTCHAS I-3 / the landed 0x0083C1E0 idiom); the
//   second def's dereference lets VC drop both destructors' null tests on the success path.
struct C3DVector { float X, Y, Z; };

struct CDefPointeeBase {                                        // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;                               // slot 1
    mutable long RefCount;                                      // +0x04
};

struct CEngineThemeDef : CDefPointeeBase {
    unsigned char _pad08[0x8c - 0x08];
    long          MaterialDef;                                  // +0x8c
};

struct CMaterialDef : CDefPointeeBase {
    unsigned char _pad08[0x4d - 0x08];
    bool          IsDiggable;                                   // +0x4d
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

void* FindDef_CDefinitionManager_GetDef_0();                    // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef_1();

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const CEngineThemeDef>* out);   // 0x0063F3A0
    bool GetDef(long index, CDefPointer<const CMaterialDef>* out);      // 0x0066C340
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CEngineThemeDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CMaterialDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CWorldMap {
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* weight) const;   // 0x0063ADF0
};

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const;             // slot 13 (+0x34)
    void*               Component;                              // +0x04 (Ego_r +0x08 after a second base vptr)
    unsigned char       _pad08[0x10 - 0x08];
    CDefinitionManager* DefinitionManager;                      // +0x10
    CDefinitionManager& GetDefinitionManager() const { return *DefinitionManager; }
};

struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                                       // +0x24 scoped_ptr<CWorld>
};
extern CMainGameComponent* g_pMainGameComponent;                // 0x013B86A0

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38();
    virtual CThing* GetThingStandingOn() const;                 // slot 39 (+0x9c)
    const C3DVector& GetPosition() const;                       // 0x004C73D0
};
class CThingCreatureBase : public CThing { };

class CTCCarriedActionUseSpade {
public:
    static bool __fastcall IsThingAbleToDig(const CThingCreatureBase& creature);
};

bool __fastcall CTCCarriedActionUseSpade::IsThingAbleToDig(const CThingCreatureBase& creature)
{
    if (!creature.GetThingStandingOn()) {
        CWorld* world = g_pMainGameComponent->PWorld;
        long weight;
        CDefPointer<const CEngineThemeDef> themeDef;
        if (world->GetDefinitionManager().GetDef(world->PeekWorldMap().GetStrongestEngineThemeAt(creature.GetPosition(), &weight), &themeDef)) {
            CDefPointer<const CMaterialDef> materialDef;
            world->GetDefinitionManager().GetDef(themeDef->MaterialDef, &materialDef);
            if (materialDef->IsDiggable)
                return true;
        }
    }
    return false;
}
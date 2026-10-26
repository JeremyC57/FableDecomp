// CThing::OnCreate() -- advance the initialisation stage; unless dead, resolve the def (marking globally loaded / persisted things), notify the thing manager and the driver, run each live component's create hook over a copy of the component map, register, and honour the cutscene pause mode
// Leaf: ?OnCreate@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9560, 293 bytes (+11 int3), virtual, `ret`, bool in al.
// Callees: CDefinitionManager::GetDef<CThingDef>(long, CDefPointer*) @ 0x0042AF3C, the thing manager's member @ 0x0051E2E0
//   (manifest "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated; name not recovered), the driver's virtual slot 29
//   (+0x74), the component map's (CVectorMap) copy constructor @ 0x004CC290, each component's virtual slot 4 (+0x10), the member
//   @ 0x00A36F80 taking the thing manager (manifest "CThingFilter_IsValid::operator()" is propagated), free @ 0x00BFEA14.
// Globals: g_pThingManager 0x013B8A1C (DefinitionManager +0x20, NonScriptedEntitesPauseMode +0x54,
//   GlobalThingLoadingBehaviour +0x80).
// Layout: the landed CThing ctor 0x004C9030 (components map +0x44, PTCDriver +0x5C, PBaseDef +0x70, DefGlobalIndex +0x8C,
//   flag bytes +0x91..+0x94); CThingBaseDef IsLoadableGlobal / GameStatePersisted are bits 0/1 of +0x42 (PDB +0x46).
// Shape: the 3-bit InitialisationStage increment is a masked read-modify-write; the def lookup goes through an inline
//   copy of GetDefinitionManager (manager loaded before the index is widened), is skipped for index 0 and its failure
//   skips the globally-loaded test; the component pass iterates a local copy of the whole 16-byte CVectorMap (a same-TU
//   stand-in copy ctor keeps its address from escaping, so begin/end stay in ebx/ebp across the virtual calls; the end is
//   a named local and each component a named pointer) and frees it; the pause mode is SetCutscenePauseBehaviour(2)'s
//   two bit stores written out.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
class CThingBaseDef { public:
    unsigned char _pad00[0x42];
    unsigned char IsLoadableGlobal : 1;          // +0x42 bit 0
    unsigned char GameStatePersisted : 1;        // bit 1
};
struct CDefPointer {                             // CDefPointer<const CThingBaseDef>
    CThingBaseDef* Object;
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer* out);   // 0x0042AF3C
};
class CThingManager { public:
    unsigned char _pad00[0x20];
    CDefinitionManager* DefinitionManager;       // +0x20
    unsigned char _pad24[0x54 - 0x24];
    bool NonScriptedEntitesPauseMode;            // +0x54
    unsigned char _pad55[0x80 - 0x55];
    long GlobalThingLoadingBehaviour;            // +0x80
    void OnThingCreated();                       // 0x0051E2E0 (name not recovered)
};
extern CThingManager* g_pThingManager;           // 0x013B8A1C
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnCreate();                     // slot 4 (+0x10)
    void* ParentThing;                           // +0x04
    bool ToBeRemoved;                            // +0x08
};
struct SComponentPair { long first; CTCBase* second; };
struct CKeyPairCompareLess { };
struct CComponentVector {                        // CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess>: vector + compare + sorted
    SComponentPair*     _M_start;
    SComponentPair*     _M_finish;
    SComponentPair*     _M_end_of_storage;
    CKeyPairCompareLess m_compare;
    bool                m_sorted;
    CComponentVector(const CComponentVector& other);   // 0x004CC290
    ~CComponentVector() { if (_M_start) free(_M_start); }
};
SComponentPair* CopyComponents_CComponentVector_0(const CComponentVector& other, SComponentPair** last);   // stand-in callee
// Stand-in body (the real copy constructor is out of line in retail): it fills the three words only inline, so VC7.1
// knows the local vector's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) CComponentVector::CComponentVector(const CComponentVector& other)
{
    SComponentPair* last;
    _M_start = CopyComponents_CComponentVector_0(other, &last);
    _M_finish = last;
    _M_end_of_storage = last;
}
#pragma auto_inline(on)
class CTCDriverBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void OnThingCreated();               // slot 29 (+0x74)
};
class CThing { public:
    virtual void v00();
    unsigned char  _pad04[0x44 - 0x04];
    CComponentVector Components;                 // +0x44 CVectorMap (0x10 with padding)
    unsigned char  _pad54[0x5c - 0x54];
    CTCDriverBase* PTCDriver;                    // +0x5c
    unsigned char  _pad60[0x70 - 0x60];
    CDefPointer    PBaseDef;                     // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefGlobalIndex;               // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  Dead : 1;                     // +0x91
    unsigned char  CreatedByOwnerThingCreation : 1;
    unsigned char  IncludeInThingScans : 1;
    unsigned char  Activated : 1;
    unsigned char  GloballyLoaded : 1;
    unsigned char  InLimbo : 1;
    unsigned char  KillLocked : 1;
    unsigned char  CreatedBySerialisation : 1;
    unsigned char  DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char  Drawable : 1;
    unsigned char  Dying : 1;
    unsigned char  ForceFrameUpdate : 1;
    unsigned char  LevelPersistent : 1;
    unsigned char  GameStatePersisted : 1;
    unsigned char  PersistOnUnloadWhenInLimbo : 1;
    unsigned char  PausedDuringCutscenes : 1;
    unsigned char  NotPausedDuringCutscenes : 1;        // +0x93
    unsigned char  DeathPersistent : 1;
    unsigned char  KillOnLevelUnload : 1;
    unsigned char  ActivatedScript : 1;
    unsigned char  LevelIsUnloading : 1;
    unsigned char  IsAffectedByBulletTimeOverride : 1;
    unsigned char  AddingTCsPermitted : 1;
    unsigned char  InitialisationStage : 3;             // +0x94
    void RegisterWithManager(CThingManager* manager);   // 0x00A36F80 (name not recovered)
    CDefinitionManager* GetDefinitionManager() const { return g_pThingManager->DefinitionManager; }   // inline copy of 0x004C7990
    bool OnCreate();
};

bool CThing::OnCreate()
{
    ++InitialisationStage;
    if (Dead)
        return false;
    if (!DefGlobalIndex || GetDefinitionManager()->GetDef(DefGlobalIndex, &PBaseDef)) {
        if ((!PBaseDef.Object || PBaseDef.Object->IsLoadableGlobal) && g_pThingManager->GlobalThingLoadingBehaviour == 1)
            GloballyLoaded = true;
    }
    if (PBaseDef.Object && PBaseDef.Object->GameStatePersisted) {
        LevelPersistent = true;
        GameStatePersisted = true;
    }
    g_pThingManager->OnThingCreated();
    if (PTCDriver)
        PTCDriver->OnThingCreated();
    CComponentVector components(Components);
    SComponentPair* end = components._M_finish;
    for (SComponentPair* it = components._M_start; it != end; ++it) {
        CTCBase* component = it->second;
        if (!component->ToBeRemoved)
            component->OnCreate();
    }
    RegisterWithManager(g_pThingManager);
    if (g_pThingManager->NonScriptedEntitesPauseMode) {
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = true;
    }
    return true;
}
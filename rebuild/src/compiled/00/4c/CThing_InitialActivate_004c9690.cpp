// CThing::InitialActivate() -- advance the initialisation stage, mark activated, join the manager's update list, run each live component's initial-activate hook over a copy of the component map, and hide things whose def is not drawable (outside the special engine mode)
// Leaf: ?InitialActivate@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9690, 274 bytes (+14 int3), virtual, `ret`.
// Callees: CThingManager::AddToUpdateList(CThing*) @ 0x0051EC50, the component map's (CVectorMap) copy constructor
//   @ 0x004CC290, each component's virtual slot 5 (+0x14), the engine singleton's virtual slot 9 (+0x24; name not
//   recovered), the def's virtual slot 1 (+4) at a zero count, this thing's virtual slot 28 (+0x70) with false,
//   free @ 0x00BFEA14.
// Globals: g_pThingManager 0x013B8A1C, the engine singleton 0x013B86A0.
// Layout: the landed CThing ctor 0x004C9030 / OnCreate 0x004C9560 (components +0x44, PBaseDef +0x70, flag bytes
//   +0x91..+0x94); CThingBaseDef IsDrawable is bit 4 of +0x41 (PDB +0x45).
// Shape: the stage increment and the Activated bit are one read-modify-write each; the component pass is the OnCreate
//   loop (same-TU stand-in copy ctor, named end); the hide test is `bool hide = !engine->X() && GetPBaseDef() &&
//   !GetPBaseDef()->IsDrawable;` with two by-value def-pointer temporaries -- VC7.1's conditional-temporary flags live in
//   ebx (1, then 3) and the frame-byte bool is tested after both releases.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
class CThingBaseDef { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long m_refCount;                             // +0x04
    unsigned char _pad08[0x41 - 0x08];
    unsigned char AvailableInEditor : 1;         // +0x41 bit 0
    unsigned char AlwaysFrameUpdate : 1;
    unsigned char StartKillLocked : 1;
    unsigned char LevelLess : 1;
    unsigned char IsDrawable : 1;                // bit 4
};
struct CDefPointer {                             // CDefPointer<const CThingBaseDef>
    CThingBaseDef* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    CThingBaseDef* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
class CThing;
class CThingManager { public:
    void AddToUpdateList(CThing* thing);         // 0x0051EC50
};
class CEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSpecialMode();                // slot 9 (+0x24; name not recovered)
};
extern CEngine* g_pEngine;                       // 0x013B86A0
extern CThingManager* g_pThingManager;           // 0x013B8A1C
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnInitialActivate();            // slot 5 (+0x14)
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
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetDrawable(bool drawable);     // slot 28 (+0x70; name not recovered)
    unsigned char  _pad04[0x44 - 0x04];
    CComponentVector Components;                 // +0x44 CVectorMap (0x10 with padding)
    unsigned char  _pad54[0x5c - 0x54];
    void*          PTCDriver;                    // +0x5c
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
    CDefPointer GetPBaseDef() const { return PBaseDef; }
    void InitialActivate();
};

void CThing::InitialActivate()
{
    ++InitialisationStage;
    Activated = true;
    g_pThingManager->AddToUpdateList(this);
    CComponentVector components(Components);
    SComponentPair* end = components._M_finish;
    for (SComponentPair* it = components._M_start; it != end; ++it) {
        CTCBase* component = it->second;
        if (!component->ToBeRemoved)
            component->OnInitialActivate();
    }
    bool hide = !g_pEngine->IsSpecialMode() && GetPBaseDef() && !GetPBaseDef()->IsDrawable;
    if (hide)
        SetDrawable(false);
}
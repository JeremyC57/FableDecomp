// CTCOpinionSource::SetupFromDef(long defIndex) -- load the opinion-source def for the index and copy its reaction switches, faction restriction (fetched as a faction def when set), produced opinion set and the 79 reaction flags; false when the def does not exist
// Grade: DIFFER(236v219) -- the copy helper keeps the def in edi plus an ebx copy (retail edi only, pushed inside the true arm), the faction branch loads g_pMainGameComponent after the pushes and the flag loop indexes [edi+eax] (retail [eax+edi])
// Retail: Fable.exe (TLC, VC7.1) @ 0x007713C0, 219 bytes (`ret 4`; PDB ?SetupFromDef@CTCOpinionSource@@AAE_NJ@Z).
// Callees: CTCBase::GetWorld() const @ 0x00686A80 -> CWorld::DefinitionManager (+0x10), CGameDefinitionManager::GetDef<COpinionSourceDef>
//   @ 0x007719C0 and GetDef<CFactionDef> @ 0x006B9D90 (both labelled GetDef<CThingPhysicalSwitchDef>), COpinionSet::operator=
//   @ 0x0045EF2D (11 landed callers name it CFloatVector::operator=; the PDB type here is COpinionSet), CMainGameComponent::
//   PeekDefinitionManager() const @ 0x00415FBC (a `jmp CDefinitionManager::Get`; manifest "GFGetPreMainMemoryUsed").
// Layout: Ego_r.pdb CTCOpinionSource with a 12-byte DefStack (PDB offsets - 4 after it); COpinionSourceDef at PDB - 8 / - 0x0c
//   (two 12-byte maps): AlwaysAwareOf..ReactToSelfPermitted +0x48..+0x4b, UseNavLineOfSight..RelativeEffect +0x5c..+0x60,
//   RestrictToFactionAndAllies +0x64, RestrictToFactionIsReversed +0x68, OpinionProduced +0x6c, ReactionFlags +0x78.
// Tried: holder in the arm (this=edi), the manager in a local (fixes the GetWorld order), a raw pointer + hand release, a
//   `__forceinline CopyFromDef(def)` helper (this=esi, as retail). Next: move the holder into the helper (one register for the
//   def) and read g_pMainGameComponent into a local before the GetDef pushes; the loop as `def->ReactionFlags[i]` on a local copy.
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
template <class T> struct CDefSlot {                    // the plain out slot GetDef fills (the reference is handed over)
    T* Object;
    CDefSlot() : Object(0) { }
};
template <class T> struct CDefHolder {                  // owns the handed-over reference
    T* Object;
    __forceinline ~CDefHolder() { ReleaseDef(Object); }
};
template <class T> struct CDefPointer {
    T* Object;
};
struct COpinionSet {                                    // 12 bytes in retail
    long _body[3];
    COpinionSet& operator=(const COpinionSet& other);   // 0x0045EF2D
};
struct COpinionSourceDef : CDefBase {                   // retail = PDB - 0x0c past the first map
    unsigned char _pad08[0x48 - 0x08];
    bool        AlwaysAwareOf;                          // +0x48
    bool        UnknownThreatSource;                    // +0x49
    bool        ReactNonstop;                           // +0x4a
    bool        ReactToSelfPermitted;                   // +0x4b
    bool        ReactionFlagDefault;                    // +0x4c
    unsigned char _pad4d[0x5c - 0x4d];                  // +0x50 ReactionFlag map (12 bytes)
    bool        UseNavLineOfSight;                      // +0x5c
    bool        WitnessesWillNotAttack;                 // +0x5d
    bool        WitnessesWillOverrideFactionEnemy;      // +0x5e
    bool        BypassesPersonality;                    // +0x5f
    bool        RelativeEffect;                         // +0x60
    long        RestrictToFactionAndAllies;             // +0x64 def index
    bool        RestrictToFactionIsReversed;            // +0x68
    COpinionSet OpinionProduced;                        // +0x6c
    bool        ReactionFlags[79];                      // +0x78
};
struct CFactionDef : CDefBase { };
struct CGameDefinitionManager {
    template <class T> bool GetDef(long index, CDefSlot<T>& out) const;      // 0x007719C0 (T = const COpinionSourceDef)
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x006B9D90 (T = const CFactionDef)
};
struct CWorld {
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x10
};
struct CMainGameComponent {
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
template <class T> struct CCountedPointer { T* Object; void* Info; };
struct CThing;
struct CTCOpinionSource {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    vector<long>  DefStack;                             // +0x0c
    bool          UseNavLineOfSight;                    // +0x18
    bool          WitnessesWillNotAttack;               // +0x19
    bool          WitnessesWillOverrideFactionEnemy;    // +0x1a
    bool          BypassesPersonality;                  // +0x1b
    bool          RelativeEffect;                       // +0x1c
    CDefPointer<const CFactionDef> RestrictToFactionAndAllies;   // +0x20
    bool          RestrictToFactionIsReversed;          // +0x24
    bool          ReactionFlags[79];                    // +0x25
    bool          ReactToSelfPermitted;                 // +0x74
    bool          AlwaysAwareOf;                        // +0x75
    bool          UnknownThreatSource;                  // +0x76
    bool          ReactNonstopFlag;                     // +0x77
    CCountedPointer<COpinionSet> POpinionProduced;      // +0x78
    const CWorld* GetWorld() const;                     // 0x00686A80 (CTCBase)
    __forceinline void CopyFromDef(const COpinionSourceDef* def)
    {
        *POpinionProduced.Object = def->OpinionProduced;
        ReactToSelfPermitted = def->ReactToSelfPermitted;
        AlwaysAwareOf = def->AlwaysAwareOf;
        UnknownThreatSource = def->UnknownThreatSource;
        ReactNonstopFlag = def->ReactNonstop;
        UseNavLineOfSight = def->UseNavLineOfSight;
        WitnessesWillNotAttack = def->WitnessesWillNotAttack;
        WitnessesWillOverrideFactionEnemy = def->WitnessesWillOverrideFactionEnemy;
        BypassesPersonality = def->BypassesPersonality;
        RelativeEffect = def->RelativeEffect;
        RestrictToFactionIsReversed = def->RestrictToFactionIsReversed;
        if (def->RestrictToFactionAndAllies)
            g_pMainGameComponent->PeekDefinitionManager().GetDef(def->RestrictToFactionAndAllies, RestrictToFactionAndAllies);
        for (unsigned long i = 0; i < 79; ++i)
            ReactionFlags[i] = def->ReactionFlags[i];
    }
    bool SetupFromDef(long defIndex);
};

bool CTCOpinionSource::SetupFromDef(long defIndex)
{
    CDefSlot<const COpinionSourceDef> fetched;
    const CGameDefinitionManager* manager = GetWorld()->DefinitionManager;
    if (manager->GetDef(defIndex, fetched)) {
        CDefHolder<const COpinionSourceDef> def = { fetched.Object };
        CopyFromDef(def.Object);
        return true;
    }
    ReleaseDef(fetched.Object);
    return false;
}

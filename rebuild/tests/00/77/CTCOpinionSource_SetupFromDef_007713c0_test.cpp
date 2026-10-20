// Behaviour test for 007713c0: src/007713c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCOpinionSource::SetupFromDef(long defIndex)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCOpinionSource::SetupFromDef(long defIndex) -- load the opinion-source def for the index and copy its reaction switches, faction restriction (fetched as a faction def when set), produced opinion set and the 79 reaction flags; false when the def does not exist
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
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
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
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefSlot<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
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
        const bool* flags = def->ReactionFlags;
        for (unsigned long i = 0; i < 79; ++i)
            ReactionFlags[i] = flags[i];
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

// ---- stubs and driver ----
static void* g_defs[2]; static int g_find0 = 0, g_find1 = 0, g_release = 0, g_destroy = 0, g_assign = 0, g_peek = 0;
static CGameDefinitionManager g_manager, g_peekManager; static CWorld g_world;
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
void* FindDef_s7r_0() { ++g_find0; return g_defs[0]; }
void* FindDef_s7r_1() { ++g_find1; return g_defs[1]; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() const { ++g_destroy; }
void CTCOpinionSource::v00() {}
COpinionSet& COpinionSet::operator=(const COpinionSet& other) { ++g_assign; _body[0] = other._body[0]; _body[1] = other._body[1]; _body[2] = other._body[2]; return *this; }
const CGameDefinitionManager& CMainGameComponent::PeekDefinitionManager() const { ++g_peek; return g_peekManager; }
const CWorld* CTCOpinionSource::GetWorld() const { return &g_world; }
int main()
{
    g_world.DefinitionManager = &g_manager;
    static COpinionSourceDef def;   // static: zero-filled members, constructed vptr
    def.m_refCount = 2; def.AlwaysAwareOf = true; def.UnknownThreatSource = false; def.ReactNonstop = true; def.ReactToSelfPermitted = true;
    def.UseNavLineOfSight = true; def.WitnessesWillNotAttack = false; def.WitnessesWillOverrideFactionEnemy = true; def.BypassesPersonality = true; def.RelativeEffect = true;
    def.RestrictToFactionAndAllies = 0; def.RestrictToFactionIsReversed = true; def.OpinionProduced._body[0] = 5; def.OpinionProduced._body[1] = 6; def.OpinionProduced._body[2] = 7;
    for (int i = 0; i < 79; ++i) def.ReactionFlags[i] = (i % 3) == 0;
    static CTCOpinionSource src;
    COpinionSet produced; produced._body[0] = produced._body[1] = produced._body[2] = 0; src.POpinionProduced.Object = &produced;
    g_defs[0] = 0; CHECK(src.SetupFromDef(4) == false); CHECK(g_find0 == 1 && g_assign == 0 && g_destroy == 0);
    g_defs[0] = &def;
    CHECK(src.SetupFromDef(4) == true);
    CHECK(g_assign == 1 && produced._body[0] == 5 && produced._body[2] == 7); CHECK(src.ReactToSelfPermitted && src.AlwaysAwareOf && !src.UnknownThreatSource && src.ReactNonstopFlag);
    CHECK(src.UseNavLineOfSight && !src.WitnessesWillNotAttack && src.WitnessesWillOverrideFactionEnemy && src.BypassesPersonality && src.RelativeEffect && src.RestrictToFactionIsReversed);
    bool flagsOk = true; for (int i = 0; i < 79; ++i) if (src.ReactionFlags[i] != ((i % 3) == 0)) flagsOk = false; CHECK(flagsOk);
    CHECK(g_peek == 0 && g_find1 == 0 && def.m_refCount == 1 && g_destroy == 0);
    // a faction restriction: fetched through the main game component's manager into the member pointer
    static CFactionDef faction; faction.m_refCount = 1; g_defs[1] = &faction; def.RestrictToFactionAndAllies = 9; def.m_refCount = 1;
    CHECK(src.SetupFromDef(4) == true); CHECK(g_peek == 1 && g_find1 == 1 && src.RestrictToFactionAndAllies.Object == &faction && def.m_refCount == 0 && g_destroy == 1);
    // missing def with a non-null slot is not possible through the stand-in (the slot stays null): the failure path releases nothing
    g_defs[0] = 0; CHECK(src.SetupFromDef(5) == false && g_destroy == 1);
    CHECK(g_release == 0);
    if (fails == 0) printf("007713c0_TEST PASS\n"); else printf("007713c0_TEST FAIL\n");
    return fails;
}
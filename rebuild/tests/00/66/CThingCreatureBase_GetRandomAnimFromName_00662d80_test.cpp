// Behaviour test for 00662d80: the row body verbatim over fixture models of the shared-header interfaces it uses (CThing::PBaseDef,
// CDefClassBase::GetSubDef<CAppearanceDef>, counted CDefPointer, CAnimationSet::AnimExists / GetAnim / GetRandomAnim, CAnimFilterNoGroup,
// the creature mode manager's current-mode filter, CThing::GetPWorldSeed).
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) { } };
class CAnimationEntry { public: const char* Name; long Group; long Index; };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { } virtual bool Accept(const CAnimationEntry& e) const = 0; };
class CAnimFilterNoGroup : public CAnimFilterBase { public: bool Accept(const CAnimationEntry& e) const { return e.Group == -1; } };
class CAnimFilterGroup : public CAnimFilterBase { public: long G; CAnimFilterGroup(long g) : G(g) { } bool Accept(const CAnimationEntry& e) const { return e.Group == G; } };
static CAnimationEntry g_entries[4] = { { "walk", 1, 0 }, { "walk", -1, 0 }, { "run", -1, 0 }, { "DEFAULT", -1, 0 } };
static long g_lastIndex = -99; static long g_lastExclude = -99; static unsigned long* g_lastSeed = 0;
class CAnimationSet { public:
    bool AnimExists(const CCharString& n, const CAnimFilterBase& f) const
    { for (int i = 0; i < 4; ++i) if (!strcmp(g_entries[i].Name, n.Text) && f.Accept(g_entries[i])) return true; return false; }
    const CAnimationEntry& GetAnim(const CCharString& n, long index, const CAnimFilterBase& f) const
    { g_lastIndex = index; for (int i = 0; i < 4; ++i) if (!strcmp(g_entries[i].Name, n.Text) && f.Accept(g_entries[i])) return g_entries[i]; return g_entries[3]; }
    const CAnimationEntry& GetRandomAnim(const CCharString& n, unsigned long& seed, const CAnimFilterBase& f, long exclude) const
    { g_lastSeed = &seed; g_lastExclude = exclude; return GetAnim(n, -7, f); }
};
struct Counted { mutable int Refs; Counted() : Refs(1) { } };
template <class T> class CDefPointer {
    T* Object;
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) --Object->Refs; }
    T* operator->() const { return Object; }
    void Set(T* p) { Object = p; if (p) ++p->Refs; }
};
class CAppearanceDef : public Counted { public: CAnimationSet Animation; };
static CAppearanceDef g_appearance;
class CThingBaseDef { public:
    mutable int Queries;
    bool GetSubDef(CDefPointer<const CAppearanceDef>* out) const { ++Queries; out->Set(&g_appearance); return true; }
};
class CModeBase { public: CAnimFilterGroup Filter; CModeBase() : Filter(1) { } const CAnimFilterBase& GetAnimFilter() const { return Filter; } };
class CTCCreatureModeManager { public: CModeBase Mode; CModeBase& GetCurrentMode() const { return const_cast<CModeBase&>(Mode); } };
static unsigned long g_seed = 1234;
class CThingCreatureBase { public:
    const CThingBaseDef* PBaseDef; CTCCreatureModeManager* PTCModeManager;
    unsigned long* GetPWorldSeed() const { return &g_seed; }
    const CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long exclude) const;
};
// ---- row body (verbatim) ----
const CAnimationEntry& CThingCreatureBase::GetRandomAnimFromName(const CCharString& name, long exclude) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode().GetAnimFilter();
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetRandomAnim(name, *GetPWorldSeed(), filter, exclude);
    if (animations.AnimExists(name, CAnimFilterNoGroup()))
        return animations.GetRandomAnim(name, *GetPWorldSeed(), CAnimFilterNoGroup(), exclude);
    animations.AnimExists("DEFAULT", CAnimFilterNoGroup());
    return animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
// ---- harness ----
int main()
{
    CThingBaseDef base; base.Queries = 0; CTCCreatureModeManager modes; CThingCreatureBase creature;
    creature.PBaseDef = &base; creature.PTCModeManager = &modes;
    CHECK(&creature.GetRandomAnimFromName("walk", 4) == &g_entries[0] && g_lastExclude == 4 && g_lastSeed == &g_seed);
    CHECK(&creature.GetRandomAnimFromName("run", 3) == &g_entries[2] && g_lastExclude == 3);
    CHECK(&creature.GetRandomAnimFromName("jump", 2) == &g_entries[3] && g_lastIndex == 0 && base.Queries == 3);
    CHECK(g_appearance.Refs == 1);
    if (fails == 0) printf("00662d80_TEST PASS\n");
    return fails;
}

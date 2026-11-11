// Behaviour test for 00662b10: the row body verbatim over fixture models of the shared-header interfaces it uses (CThing::PBaseDef,
// CDefClassBase::GetSubDef<CAppearanceDef>, counted CDefPointer, CAnimationSet::AnimExists / GetAnim, CAnimFilterNoGroup).
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

class CThingCreatureBase { public:
    const CThingBaseDef* PBaseDef;
    const CAnimationEntry& GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index) const;
};
// ---- row body (verbatim) ----
const CAnimationEntry& CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetAnim(name, index, filter);
    if (animations.AnimExists(name, CAnimFilterNoGroup()))
        return animations.GetAnim(name, index, CAnimFilterNoGroup());
    return animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
// ---- harness ----
int main()
{
    CThingBaseDef base; base.Queries = 0; CThingCreatureBase creature; creature.PBaseDef = &base;
    CHECK(&creature.GetAnimFromName("walk", CAnimFilterGroup(1), 5) == &g_entries[0] && g_lastIndex == 5);
    CHECK(&creature.GetAnimFromName("run", CAnimFilterGroup(1), 6) == &g_entries[2] && g_lastIndex == 6);
    CHECK(&creature.GetAnimFromName("jump", CAnimFilterGroup(1), 7) == &g_entries[3] && g_lastIndex == 0 && base.Queries == 3);
    CHECK(g_appearance.Refs == 1);
    if (fails == 0) printf("00662b10_TEST PASS\n");
    return fails;
}

// Behaviour test for 006631c0: the row body verbatim over fixture models of the shared-header interfaces (CDefClassBase::GetSubDef<CAppearanceDef>, CThing::PeekWorld, CWorld::PeekMeshBank, CMeshDataBank::PeekAnimation, CAnimationSet queries, C3DAnimationInfo loading).
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (count starts at 1 for the manager's own reference, Delete counted),
// the counted CDefPointer handle, and the definition manager's GetDef<T> (fills the handle with an add-ref, returns whether found).
class CDefPointeeBase { public:
    long Refs; int Deleted;
    CDefPointeeBase() : Refs(1), Deleted(0) { }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { if (Object) { if (--Object->Refs == 0) Object->Delete(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (Object && --Object->Refs == 0) Object->Delete(); Object = p; if (p) ++p->Refs; }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
#include <vector>
#include <utility>
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) { } };
static bool StartsWith(const char* s, const char* p) { return strncmp(s, p, strlen(p)) == 0; }
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { } };
class CAnimFilterNoGroup : public CAnimFilterBase { };
class CAnimationEntry { public:
    long BankIndex; const char* Name; long Flags;
    long GetFlags() const { return Flags; }
};
class CAnimationSet { public:
    typedef std::vector<std::pair<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;
    long GetNoAnims(const CCharString& name, const CAnimFilterBase&) const
    {
        long n = 0;
        for (unsigned i = 0; i < Anims.size(); ++i) if (strcmp(Anims[i].second.Name, name.Text) == 0) ++n;
        return n;
    }
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase&) const
    {
        for (unsigned i = 0; i < Anims.size(); ++i) if (strcmp(Anims[i].second.Name, name.Text) == 0 && index-- == 0) return Anims[i].second;
        return Anims[0].second;
    }
    void GetAnimsWithNameStartingWith(const CCharString& prefix, std::vector<const CAnimationEntry*>& out, const CAnimFilterBase&) const
    {
        for (unsigned i = 0; i < Anims.size(); ++i) if (StartsWith(Anims[i].second.Name, prefix.Text)) out.push_back(&Anims[i].second);
    }
};
class C3DAnimationInfo { public:
    int Touched; int Loads; bool LastFlag;
    void TouchAnimation() const { ++const_cast<C3DAnimationInfo*>(this)->Touched; }
    void BeginLoading(bool flag) const { C3DAnimationInfo* self = const_cast<C3DAnimationInfo*>(this); ++self->Loads; self->LastFlag = flag; }
};
static C3DAnimationInfo g_infos[16];
class CMeshDataBank { public: const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return &g_infos[index]; } };
static CMeshDataBank g_bank;
class CWorld { public: const CMeshDataBank* PeekMeshBank() const { return &g_bank; } };
static CWorld g_world;
class CAppearanceDef : public CDefPointeeBase { public: CAnimationSet Animation; };
static CAppearanceDef* g_appearance = 0;
class CThingBaseDef : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const { if (!g_appearance) return false; out->Set(g_appearance); return true; }
};
class CThingCreatureBase { public:
    CDefPointer<const CThingBaseDef> PBaseDef;
    const CWorld& PeekWorld() const { return g_world; }
    void PreloadAnims();
    void PreloadAnimation(const CCharString& name);
    void PreloadBasicAnimations();
};
static void AddAnim(CAppearanceDef& app, long bank, const char* name, long flags)
{
    CAnimationEntry e; e.BankIndex = bank; e.Name = name; e.Flags = flags;
    app.Animation.Anims.push_back(std::make_pair((unsigned long)app.Animation.Anims.size(), e));
}
static void ResetInfos() { memset(g_infos, 0, sizeof(g_infos)); }
// ---- row body (verbatim) ----
void CThingCreatureBase::PreloadAnimation(const CCharString& name)
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance)) {
        const CMeshDataBank* bank = PeekWorld().PeekMeshBank();
        const CAnimationSet& set = appearance->Animation;
        long count = set.GetNoAnims(name, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->PeekAnimation(set.GetAnim(name, i, CAnimFilterNoGroup()).BankIndex)->TouchAnimation();
    }
}
// ---- harness ----
int main()
{
    CThingBaseDef base; CAppearanceDef app; CThingCreatureBase c; c.PBaseDef.Set(&base); ResetInfos();
    AddAnim(app, 1, "ST_WALK", 4); AddAnim(app, 2, "ST_RUN", 0); AddAnim(app, 3, "ST_WALK", 4); AddAnim(app, 4, "TURNING_LEFT", 4); AddAnim(app, 5, "IDLE", 0);
    c.PreloadAnimation(CCharString("ST_WALK")); CHECK(g_infos[1].Touched == 0);
    g_appearance = &app; c.PreloadAnimation(CCharString("ST_WALK"));
    CHECK(g_infos[1].Touched == 1 && g_infos[3].Touched == 1); CHECK(g_infos[2].Touched == 0 && g_infos[4].Touched == 0);
    CHECK(app.Refs == 1); c.PBaseDef.Object = 0; g_appearance = 0;
    if (fails == 0) printf("006631c0_TEST PASS\n");
    return fails;
}

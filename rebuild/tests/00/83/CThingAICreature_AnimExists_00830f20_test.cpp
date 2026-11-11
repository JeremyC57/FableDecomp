// Behaviour test for 00830f20: the row body verbatim over fixture models of the shared-header interfaces (CDefClassBase::GetSubDef<CAppearanceDef>, CTCCreatureModeManager::GetCurrentMode and the mode filter, CAnimationSet::AnimExists).
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
class CCharString { public: const char* Text; CCharString(const char* t, long = -1) : Text(t) { } };
class CAnimFilterBase { public: int Group; CAnimFilterBase() : Group(-1) { } virtual ~CAnimFilterBase() { } };
class CAnimFilterNoGroup : public CAnimFilterBase { };
static int g_modeGroup = 7;
class CAnimationSet { public:
    const char* Names[4]; int Groups[4]; int Count;
    // an entry matches the mode filter when its group is the mode's, and the no-group filter when it has no group (-1)
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const
    {
        for (int i = 0; i < Count; ++i) if (strcmp(Names[i], name.Text) == 0 && Groups[i] == filter.Group) return true;
        return false;
    }
};
class CTCCreatureModeManager { public:
    class CModeBase { public: CAnimFilterBase Filter; const CAnimFilterBase& GetAnimFilter() const { return Filter; } };
    CModeBase Mode; int Calls;
    CModeBase& GetCurrentMode() const { ++const_cast<CTCCreatureModeManager*>(this)->Calls; return const_cast<CModeBase&>(Mode); }
};
class CAppearanceDef : public CDefPointeeBase { public: CAnimationSet Animation; };
static CAppearanceDef* g_appearance = 0;
class CThingBaseDef : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const { if (!g_appearance) return false; out->Set(g_appearance); return true; }
};
class CThingCreatureBase { public:
    CDefPointer<const CThingBaseDef> PBaseDef; CTCCreatureModeManager* PTCModeManager;
    bool AnimExists(const CCharString& name, bool useModeFilterOnly) const;
};
class CThingAICreature : public CThingCreatureBase { public:
    bool AnimExists(const CCharString& name, bool useModeFilterOnly) const;
};
// ---- row body (verbatim) ----
bool CThingAICreature::AnimExists(const CCharString& name, bool useModeFilterOnly) const
{
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    const CTCCreatureModeManager::CModeBase& mode = PTCModeManager->GetCurrentMode();
    const CAnimationSet& set = appearance->Animation;
    if (set.AnimExists(name, mode.GetAnimFilter()))
        return true;
    if (!useModeFilterOnly)
        return set.AnimExists(name, CAnimFilterNoGroup());
    return false;
}
// ---- harness ----
int main()
{
    CThingBaseDef base; CAppearanceDef app; g_appearance = &app; CTCCreatureModeManager modes; modes.Calls = 0; modes.Mode.Filter.Group = 7;
    app.Animation.Count = 2; app.Animation.Names[0] = "SWING"; app.Animation.Groups[0] = 7; app.Animation.Names[1] = "IDLE"; app.Animation.Groups[1] = -1;
    CThingAICreature c; c.PBaseDef.Set(&base); c.PTCModeManager = &modes;
    CHECK(c.AnimExists(CCharString("SWING"), true)); CHECK(modes.Calls == 1);
    CHECK(!c.AnimExists(CCharString("IDLE"), true)); CHECK(c.AnimExists(CCharString("IDLE"), false));
    CHECK(!c.AnimExists(CCharString("NONE"), false)); CHECK(app.Refs == 1);
    c.PBaseDef.Object = 0; g_appearance = 0;
    if (fails == 0) printf("00830f20_TEST PASS\n");
    return fails;
}

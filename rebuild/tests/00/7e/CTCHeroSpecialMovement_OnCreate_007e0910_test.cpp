// Behaviour test for 007e0910: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCHeroSpecialMovement::OnCreate() -- look up the parent thing's def by its def index and take its hero special-movement sub-def; then clear Sprinting and preset the sprint frames from the def's sprint duration
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (Refs counts references, Delete counted), the counted CDefPointer
// handle (copy / assignment add-ref, release on destruction), the definition manager's GetDef<T> (fills the handle with an add-ref,
// returns whether found) and CDefClassBase::GetSubDef<T> (the sub-def table of the def, by a type slot of T).
class CDefPointeeBase { public:
    long Refs; int Deleted; CDefPointeeBase* Sub[8];
    CDefPointeeBase() : Refs(1), Deleted(0) { memset(Sub, 0, sizeof(Sub)); }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { Release(); }
    CDefPointer& operator=(const CDefPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->Refs; } return *this; }
    void Release() { if (Object) { if (--Object->Refs == 0) Object->Delete(); Object = 0; } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    T* Get() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (p) ++p->Refs; Release(); Object = p; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const
    {
        CDefPointeeBase* p = Sub[T::Slot];
        if (!p) return false;
        out->Set(p); return true;
    }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    static CGameDefinitionManager& Get();
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
CGameDefinitionManager& CGameDefinitionManager::Get() { return g_manager; }
static void ResetManager() { memset(&g_manager, 0, sizeof(g_manager)); }
class CThingBaseDef : public CDefClassBase { public: enum { Slot = 0 }; };
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_DEFAULT = 0, CUTSCENE_BEHAVIOUR_PAUSED = 1, CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2 };
class CThing { public:
    unsigned short DefGlobalIndex; long CutscenePause;
    CDefPointer<const CThingBaseDef> PBaseDef;
    unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    void SetCutscenePauseBehaviour(ECutsceneBehaviour b) { CutscenePause = b; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
};
class CTCBase { public:
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; int BaseCalls;
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false), BaseCalls(0) { }
    void OnCreate() { ++BaseCalls; }
    void OnInitialActivate() { ++BaseCalls; }
    const CGameDefinitionManager& GetDefinitionManager() const { return g_manager; }
};
class CHeroSpecialMovementDef : public CDefClassBase { public: enum { Slot = 4 }; float TotalTimeToSprintSeconds; };
class CMainGameComponent { public: static long ConstantFPS; static long GetConstantFPS() { return ConstantFPS; } };
long CMainGameComponent::ConstantFPS = 30;
class CTCHeroSpecialMovement : public CTCBase { public:
    bool Sprinting; bool Running; float SprintFramesRemaining; long RunTimer; CDefPointer<const CHeroSpecialMovementDef> PHeroSpecialMovementDef;
    CTCHeroSpecialMovement(CThing& thing) : CTCBase(thing), Sprinting(true), Running(false), SprintFramesRemaining(0), RunTimer(0) { }
    void OnCreate();
};
// ---- row body (verbatim) ----
void CTCHeroSpecialMovement::OnCreate()
{
    unsigned short def_index = (unsigned short)ParentThing->GetDefGlobalIndex();
    if (def_index) {
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(def_index, &def);
        def->GetSubDef(&PHeroSpecialMovementDef);
    }
    Sprinting = false;
    SprintFramesRemaining = (float)CMainGameComponent::GetConstantFPS() * PHeroSpecialMovementDef->TotalTimeToSprintSeconds;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CHeroSpecialMovementDef sub; sub.TotalTimeToSprintSeconds = 2.5f; thingDef.Sub[4] = &sub; g_manager.Table[6] = &thingDef;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.DefGlobalIndex = 6;
    {
        CTCHeroSpecialMovement c(thing);
        c.OnCreate();
        CHECK(!c.Sprinting && c.PHeroSpecialMovementDef.Object == &sub && sub.Refs == 2 && thingDef.Refs == 1);
        CHECK(c.SprintFramesRemaining > 74.99f && c.SprintFramesRemaining < 75.01f && g_manager.Calls == 1);
        thing.DefGlobalIndex = 0;                            // no def index: no lookup, the held def is kept
        c.OnCreate();
        CHECK(g_manager.Calls == 1 && c.PHeroSpecialMovementDef.Object == &sub && c.SprintFramesRemaining > 74.99f);
    }
    CHECK(sub.Refs == 1 && sub.Deleted == 0);
    if (fails == 0) printf("007e0910_TEST PASS\n");
    return fails;
}

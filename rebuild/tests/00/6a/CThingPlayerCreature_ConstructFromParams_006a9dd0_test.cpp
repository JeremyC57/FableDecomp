// Behaviour test for 006a9dd0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const CThingPlayerCreatureInit& init) -- report progress, construct the creature base, reset health from the creature def, look up the appearance sub-def and add the physics-controlled component
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
#include <string>
#include <vector>
class CCharString { public: std::string Text; CCharString(const char* t) : Text(t) { } };
class C3DVector { public: float X, Y, Z; };
class CThingPhysical;
static std::vector<std::string> g_progress;
namespace NProgressDisplay { void DisplayProgress(const CCharString& text, float progress, bool, bool) { if (progress == -1.0f) g_progress.push_back(text.Text); } }
class CAppearanceDef : public CDefClassBase { public: enum { Slot = 1 }; };
class CThingCreatureDef : public CThingBaseDef { public: float Health; };
class CThingPlayerCreatureInit { public: CThingPhysical* OriginalThing; CThingPhysical* PreviousThing; bool PlaceAtEndOfList; };
static bool g_baseOk = true; static long g_baseArgs[3]; static bool g_baseFlag; static std::vector<std::string> g_added;
class CThingCreatureBase { public:
    CDefPointer<const CThingBaseDef> PBaseDef; CDefPointer<const CThingCreatureDef> PDef; float MaxHealth, Health;
    bool ConstructFromParams(long defIndex, const C3DVector&, long playerNumber, CThingPhysical* creator, CThingPhysical*, const CCharString& script, bool flag)
    { g_baseArgs[0] = defIndex; g_baseArgs[1] = playerNumber; g_baseArgs[2] = (long)(creator != 0); g_baseFlag = flag; return g_baseOk && script.Text.empty(); }
    void* AddTC(const CCharString& name, bool, const void*) { g_added.push_back(name.Text); return 0; }
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const CThingPlayerCreatureInit& init);
};
// ---- row body (verbatim) ----
bool CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const CThingPlayerCreatureInit& init)
{
    NProgressDisplay::DisplayProgress("CThingPlayerCreature::ConstructFromParams 1", -1.0f, false, false);
    if (CThingCreatureBase::ConstructFromParams(defIndex, position, playerNumber, init.OriginalThing, init.PreviousThing, "", init.PlaceAtEndOfList)) {
        NProgressDisplay::DisplayProgress("CThingPlayerCreature::ConstructFromParams 2", -1.0f, false, false);
        MaxHealth = Health = PDef->Health;
        CDefPointer<const CAppearanceDef> appearance;
        PBaseDef->GetSubDef(&appearance);
        AddTC("CTCPhysicsControlled", true, 0);
        return true;
    }
    return false;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CAppearanceDef app; thingDef.Sub[1] = &app;
    CThingCreatureDef creatureDef; creatureDef.Health = 75.0f;
    CThingPlayerCreature hero; hero.PBaseDef.Object = &thingDef; hero.PDef.Object = &creatureDef; hero.MaxHealth = hero.Health = 1.0f;
    C3DVector pos = { 1, 2, 3 };
    CThingPlayerCreatureInit init = { (CThingPhysical*)&pos, 0, true };
    CHECK(hero.ConstructFromParams(12, pos, 3, init));
    CHECK(g_progress.size() == 2 && g_progress[0] == "CThingPlayerCreature::ConstructFromParams 1" && g_progress[1] == "CThingPlayerCreature::ConstructFromParams 2");
    CHECK(g_baseArgs[0] == 12 && g_baseArgs[1] == 3 && g_baseArgs[2] == 1 && g_baseFlag);
    CHECK(hero.MaxHealth == 75.0f && hero.Health == 75.0f && g_added.size() == 1 && g_added[0] == "CTCPhysicsControlled");
    CHECK(app.Refs == 1 && app.Deleted == 0);
    g_baseOk = false; g_progress.clear(); g_added.clear(); hero.Health = 5.0f;
    CHECK(!hero.ConstructFromParams(12, pos, 3, init));
    CHECK(g_progress.size() == 1 && g_added.empty() && hero.Health == 5.0f);
    hero.PBaseDef.Object = 0; hero.PDef.Object = 0;
    if (fails == 0) printf("006a9dd0_TEST PASS\n");
    return fails;
}

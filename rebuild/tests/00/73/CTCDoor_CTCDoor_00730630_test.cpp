// Behaviour test for 00730630: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCDoor::CTCDoor(CThing& thing) -- CTCBase(thing), not jammed, closed and still, no creature/update stamps, then the door def fetched from the parent's definition and the parent's cutscene pause behaviour set to not paused
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
class CDoorDef : public CDefClassBase { public: enum { Slot = 3 }; };
enum EDoorTriggerType { DOOR_TRIGGER_NONE };
class CTCDoor : public CTCBase { public:
    long Jammed; bool CurrentOpen, OldOpen, Opening, Closing; long LastCreatureFoundWF; long UpdateID; EDoorTriggerType TriggerType;
    CDefPointer<const CDoorDef> PDef;
    CTCDoor(CThing& thing);
};
// ---- row body (verbatim) ----
CTCDoor::CTCDoor(CThing& thing)
    : CTCBase(thing)
    , Jammed(0)
    , CurrentOpen(false)
    , OldOpen(false)
    , Opening(false)
    , Closing(false)
    , LastCreatureFoundWF(0)
    , UpdateID(0)
    , PDef()
{
    CDefPointer<const CThingBaseDef> def;
    CThing* parent = ParentThing;
    GetDefinitionManager().GetDef(parent->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PDef);
    ParentThing->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CDoorDef doorDef; thingDef.Sub[3] = &doorDef; g_manager.Table[6] = &thingDef;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.DefGlobalIndex = 6;
    {
        CTCDoor door(thing);
        CHECK(door.ParentThing == &thing && door.Jammed == 0 && !door.CurrentOpen && !door.OldOpen && !door.Opening && !door.Closing);
        CHECK(door.LastCreatureFoundWF == 0 && door.UpdateID == 0 && door.PDef.Object == &doorDef && doorDef.Refs == 2);
        CHECK(thingDef.Refs == 1 && thing.CutscenePause == CUTSCENE_BEHAVIOUR_NOT_PAUSED && g_manager.LastIndex == 6);
    }
    CHECK(doorDef.Refs == 1 && doorDef.Deleted == 0);
    if (fails == 0) printf("00730630_TEST PASS\n");
    return fails;
}

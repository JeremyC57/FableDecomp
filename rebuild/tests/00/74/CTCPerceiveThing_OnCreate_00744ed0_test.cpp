// Behaviour test for 00744ed0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCPerceiveThing::OnCreate() -- fetch the parent's thing def, take its CPerceivedThingDef sub-def into PDef, reset the perception state, load the four radii from the def (keeping ExtendedSightRadius >= SightRadius), the give-up-chase radius, and clear DirectAwareness
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
class CPerceivedThingDef : public CDefClassBase { public: enum { Slot = 4 };
    float FieldOfView, GiveUpChaseRadius, SightRadius, ExtendedSightRadius, SoundRadius, SmellRadius; };
class CTCPerceiveThing : public CTCBase { public:
    bool DirectAwareness; CDefPointer<const CPerceivedThingDef> PDef;
    float SoundRadius, SightRadius, ExtendedSightRadius, SmellRadius, GiveUpChaseRadius; int Resets;
    CTCPerceiveThing(CThing& thing) : CTCBase(thing), DirectAwareness(true), SoundRadius(0), SightRadius(0), ExtendedSightRadius(0),
        SmellRadius(0), GiveUpChaseRadius(0), Resets(0) { }
    void ResetPerceptionVariables() { ++Resets; }
    void SetSoundRadius(float radius); void SetSmellRadius(float radius); void SetSightRadius(float radius);
    void SetExtendedSightRadius(float radius);
    void OnCreate();
};
// ---- row body (verbatim) ----
void CTCPerceiveThing::SetSoundRadius(float radius)
{
    if (radius != -1.0f)
        SoundRadius = radius;
    else
        SoundRadius = PDef->SoundRadius;
}

void CTCPerceiveThing::SetSmellRadius(float radius)
{
    if (radius != -1.0f)
        SmellRadius = radius;
    else
        SmellRadius = PDef->SmellRadius;
}

void CTCPerceiveThing::SetSightRadius(float radius)
{
    if (radius != -1.0f) {
        SightRadius = radius;
        return;
    }
    SightRadius = PDef->SightRadius;
    if (SightRadius > ExtendedSightRadius)
        ExtendedSightRadius = SightRadius;
}

void CTCPerceiveThing::SetExtendedSightRadius(float radius)
{
    if (radius != -1.0f) {
        ExtendedSightRadius = radius;
        return;
    }
    ExtendedSightRadius = PDef->ExtendedSightRadius;
    if (ExtendedSightRadius < SightRadius)
        ExtendedSightRadius = SightRadius;
}

void CTCPerceiveThing::OnCreate()
{
    CThing* thing = ParentThing;
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PDef);
    ResetPerceptionVariables();
    SetSightRadius(-1.0f);
    SetSoundRadius(-1.0f);
    SetSmellRadius(-1.0f);
    SetExtendedSightRadius(-1.0f);
    GiveUpChaseRadius = PDef->GiveUpChaseRadius;
    DirectAwareness = false;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef thingDef; CPerceivedThingDef p; thingDef.Sub[4] = &p; g_manager.Table[6] = &thingDef;
    p.GiveUpChaseRadius = 30; p.SightRadius = 20; p.ExtendedSightRadius = 10; p.SoundRadius = 5; p.SmellRadius = 3;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.DefGlobalIndex = 6;
    {
        CTCPerceiveThing c(thing);
        c.OnCreate();                                          // extended (10) below sight (20): raised to sight
        CHECK(c.PDef.Object == &p && c.Resets == 1 && c.SightRadius == 20 && c.ExtendedSightRadius == 20 && c.SoundRadius == 5);
        CHECK(c.SmellRadius == 3 && c.GiveUpChaseRadius == 30 && !c.DirectAwareness && p.Refs == 2 && thingDef.Refs == 1);
        p.ExtendedSightRadius = 40;
        c.OnCreate();
        CHECK(c.SightRadius == 20 && c.ExtendedSightRadius == 40 && c.Resets == 2 && p.Refs == 2);
        c.SetSightRadius(7.0f); c.SetSoundRadius(8.0f);
        CHECK(c.SightRadius == 7.0f && c.SoundRadius == 8.0f && c.ExtendedSightRadius == 40);
    }
    CHECK(p.Refs == 1 && p.Deleted == 0);
    if (fails == 0) printf("00744ed0_TEST PASS\n");
    return fails;
}

// Behaviour test for 0070b4d0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCAnimationComplex::OnCreate() -- reset the thing's animator and start the appearance's first "DEFAULT" animation as the main anim (type 6, mode 1, not looping, percent 100)
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
// Fixture models of the animation interfaces: CCharString (a name), the appearance def's animation set (named entries; GetAnim
// returns the index-th entry with that name, recording the filter kind), the animation request (CAnimInit's argument constructor
// records its arguments), the animator (Init / SetMainAnim / ClearAllAnims recorded), the animation component, and a CThing with a
// component table indexed by interface type, a base def handle and a definition index.
class CCharString { public:
    const char* Text;
    CCharString(const char* text = "", long = -1) : Text(text) { }
    bool operator==(const CCharString& o) const { return strcmp(Text, o.Text) == 0; }
};
enum EAnimUpdateMode { ANIM_UPDATE_NONE = 0, ANIM_UPDATE_FROM_SPEED = 1, ANIM_UPDATE_FROM_DIST = 2 };
class CAnimationEntry { public:
    long BankIndex; const char* Name; unsigned long Flags; long TransitionTime;
    unsigned long GetAnimateThingFlags() const { return Flags; }
    long GetTransitionTime() const { return TransitionTime; }
};
class CAnimFilterBase { public: int Kind; CAnimFilterBase(int kind = 0) : Kind(kind) { } };
class CAnimFilterNoGroup : public CAnimFilterBase { public: CAnimFilterNoGroup() : CAnimFilterBase(1) { } };
static int g_lastFilterKind = -1;
class CAnimationSet { public:
    CAnimationEntry Entries[8]; int Count;
    const CAnimationEntry& GetAnim(const CCharString& name, long index, const CAnimFilterBase& filter) const
    {
        g_lastFilterKind = filter.Kind;
        for (int i = 0; i < Count; ++i)
            if (strcmp(Entries[i].Name, name.Text) == 0 && index-- == 0) return Entries[i];
        return Entries[0];
    }
    bool AnimExists(const CCharString& name, const CAnimFilterBase&) const
    {
        for (int i = 0; i < Count; ++i) if (strcmp(Entries[i].Name, name.Text) == 0) return true;
        return false;
    }
};
class CAppearanceDef : public CDefClassBase { public: enum { Slot = 1 }; CAnimationSet Animation; };
class CAnimFrameInfo { public: float Current, Last; CAnimFrameInfo() : Current(0), Last(0) { } };
class CAnimInit { public:
    unsigned char Type; unsigned long TypeFlags; long Anim; EAnimUpdateMode UpdateMode; bool Looping; unsigned long Flags; long Priority;
    CAnimFrameInfo Frame;
    CAnimInit() { }
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority)
        : Type(type), TypeFlags(typeFlags), Anim(anim), UpdateMode(updateMode), Looping(looping), Flags(flags), Priority(priority) { }
};
class CAnimateThing { public:
    CAnimInit Last; int Inits; int MainAnims; int Clears; long Arg1, Arg2;
    void Init(const CAnimInit& init) { Last = init; ++Inits; }
    void SetMainAnim(const CAnimInit& init, long a, long b) { Last = init; ++MainAnims; Arg1 = a; Arg2 = b; }
    void ClearAllAnims() { ++Clears; }
};
enum ETCInterfaceType { TCI_ANIMATION = 90 };
class CTCBase;
class CThing { public:
    CTCBase* Components[128];
    unsigned short DefGlobalIndex;
    CDefPointer<const CThingBaseDef> PBaseDef;
    bool HasTC(ETCInterfaceType type) const { return Components[type] != 0; }
    template <class T> bool GetTC(T** out) { if (!Components[T::StatGetInterfaceType()]) return false; *out = (T*)Components[T::StatGetInterfaceType()]; return true; }
    template <class T> bool PeekTC(const T** out) const { if (!Components[T::StatGetInterfaceType()]) return false; *out = (const T*)Components[T::StatGetInterfaceType()]; return true; }
    unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
};
class CTCBase { public:
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    const CGameDefinitionManager& GetDefinitionManager() const { return g_manager; }
};
class CTCAnimationComplex : public CTCBase { public:
    CAnimateThing Animator; CAnimateThing* PAnimation;
    CTCAnimationComplex(CThing& thing) : CTCBase(thing), PAnimation(&Animator) { memset(&Animator, 0, sizeof(Animator)); }
    void OnCreate();
    static ETCInterfaceType StatGetInterfaceType() { return TCI_ANIMATION; }
    CAnimateThing& GetAnimation() { return Animator; }
};

// ---- row body (verbatim) ----
void CTCAnimationComplex::OnCreate()
{
    CAnimateThing* animation = PAnimation;
    animation->ClearAllAnims();
    const CThingBaseDef* base_def = ParentThing->GetPBaseDef().Get();
    CDefPointer<const CAppearanceDef> appearance;
    base_def->GetSubDef(&appearance);
    const CAnimationEntry& entry = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, false, 0, 100);
    animation->SetMainAnim(init, 1, 0);
}
// ---- harness ----
// shared harness setup: a thing with a base def whose appearance sub-def has DEFAULT / named entries, and an animation component
static CThingBaseDef thingDef; static CAppearanceDef app; static CThing thing;
static void SetupThing(const char* name, long bank)
{
    ResetManager();
    memset(&app.Animation, 0, sizeof(app.Animation)); app.Refs = 1; app.Deleted = 0;
    thingDef.Refs = 1; thingDef.Deleted = 0; thingDef.Sub[1] = &app;
    CAnimationEntry e0 = { 3, "DEFAULT", 0x10, 7 }, e1 = { bank, name, 0x20, 9 };
    app.Animation.Entries[0] = e0; app.Animation.Entries[1] = e1; app.Animation.Count = 2;
    memset(&thing, 0, sizeof(thing)); thing.PBaseDef.Object = &thingDef;
}
int main()
{
    SetupThing("WAVE", 21);
    CTCAnimationComplex complex(thing);
    complex.OnCreate();
    CHECK(complex.Animator.Clears == 1 && complex.Animator.MainAnims == 1 && complex.Animator.Arg1 == 1 && complex.Animator.Arg2 == 0);
    CHECK(complex.Animator.Last.Anim == 3 && complex.Animator.Last.Type == 6 && !complex.Animator.Last.Looping);
    CHECK(complex.Animator.Last.UpdateMode == ANIM_UPDATE_FROM_SPEED && complex.Animator.Last.Priority == 100);
    CHECK(g_lastFilterKind == 1 && app.Refs == 1 && app.Deleted == 0 && thingDef.Refs == 1 && thingDef.Deleted == 0);
    thing.PBaseDef.Object = 0;
    if (fails == 0) printf("0070b4d0_TEST PASS\n");
    return fails;
}

// Behaviour test for 00736640: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCCombat::AddMeleeFlourishAbility(long defIndex) -- looks up a melee combat ability def, appends it to AbilityList and recomputes the usable abilities
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
class CMeleeCombatAbilityDef : public CDefClassBase { public: enum { Slot = 5 }; };
template <class T> class CList8 { public:
    T Data[8]; unsigned int Count; CList8() : Count(0) { }
    void push_back(const T& v) { Data[Count++] = v; }
};
class CTCCombat { public:
    CList8<CDefPointer<const CMeleeCombatAbilityDef> > AbilityList; int Updates;
    const CGameDefinitionManager& GetDefinitionManager() const { return g_manager; }
    void UpdateUsableMeleeCombatAbilities() { ++Updates; }
    void AddMeleeFlourishAbility(long def_index);
};
// ---- row body (verbatim) ----
void CTCCombat::AddMeleeFlourishAbility(long def_index)
{
    CDefPointer<const CMeleeCombatAbilityDef> def;
    GetDefinitionManager().GetDef(def_index, &def);
    AbilityList.push_back(def);
    UpdateUsableMeleeCombatAbilities();
}
// ---- harness ----
int main()
{
    ResetManager();
    CMeleeCombatAbilityDef a; g_manager.Table[5] = &a;
    {
        CTCCombat c; c.Updates = 0;
        c.AddMeleeFlourishAbility(5);
        CHECK(c.AbilityList.Count == 1 && c.AbilityList.Data[0].Object == &a && a.Refs == 2 && c.Updates == 1 && g_manager.LastIndex == 5);
        c.AddMeleeFlourishAbility(9);                          // unknown def: a null entry is still appended
        CHECK(c.AbilityList.Count == 2 && c.AbilityList.Data[1].IsNull() && c.Updates == 2);
    }
    CHECK(a.Refs == 1 && a.Deleted == 0);
    if (fails == 0) printf("00736640_TEST PASS\n");
    return fails;
}

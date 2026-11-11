// Behaviour test for 0089e080: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CGameScriptInterface::EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const -- for a live thing with a combat component, look the combat type def up by name and set it
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
static CDefPointeeBase* (*g_nameLookup)(const char*) = 0;   // by-name definition lookup hook (set by the harness)
class CGameDefinitionManager { public:
    template <class N, class T> bool GetDef(const N& name, CDefPointer<const T>* out) const
    {
        CDefPointeeBase* p = g_nameLookup ? g_nameLookup(name.Text) : 0;
        if (!p) return false;
        out->Set(p); return true;
    }
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
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CCombatTypeDef : public CDefClassBase { };
static CCombatTypeDef g_typeDefs[2]; static const char* g_typeNames[2] = { "COMBAT_SWORD", "COMBAT_STAFF" };
static CDefPointeeBase* LookupType(const char* name)
{
    for (int i = 0; i < 2; ++i) if (strcmp(name, g_typeNames[i]) == 0) return &g_typeDefs[i];
    return 0;
}
class CTCCombat { public:
    CDefPointer<const CCombatTypeDef> PCombatTypeDef; int Sets;
    static int StatGetInterfaceType() { return 0x30; }
    void SetCombatType(const CDefPointer<const CCombatTypeDef>& def) { PCombatTypeDef = def; ++Sets; }
};
class CThing { public:
    bool Dead; CTCCombat* Combat;
    bool IsDead() const { return Dead; }
    template <class T> bool GetTC(T** out) { if (!Combat) return false; *out = Combat; return true; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
};
class CScriptThing { public: CThing* Thing; virtual CThing* GetPThing() const { return Thing; } };
class CGameScriptInterface { public:
    virtual void EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const;
};
// ---- row body (verbatim) ----
void CGameScriptInterface::EntitySetCombatType(const CScriptThing& thing, const CCharString& combat_type) const
{
    CThing* pthing = thing.GetPThing();
    CDefPointer<const CCombatTypeDef> type_def;
    CTCCombat* combat;
    if (pthing && !pthing->IsDead() && pthing->GetTC(&combat)) {
        if (pthing->PeekDefinitionManager().GetDef(combat_type, &type_def))
            combat->SetCombatType(type_def);
    }
}
// ---- harness ----
int main()
{
    g_nameLookup = LookupType;
    CTCCombat combat; combat.Sets = 0;
    CThing thing = { false, &combat };
    CScriptThing script; script.Thing = &thing;
    CGameScriptInterface gsi;
    gsi.EntitySetCombatType(script, "COMBAT_STAFF");
    CHECK(combat.Sets == 1 && combat.PCombatTypeDef.Object == &g_typeDefs[1] && g_typeDefs[1].Refs == 2);
    gsi.EntitySetCombatType(script, "NO_SUCH_TYPE");
    CHECK(combat.Sets == 1 && combat.PCombatTypeDef.Object == &g_typeDefs[1]);
    thing.Dead = true; gsi.EntitySetCombatType(script, "COMBAT_SWORD"); CHECK(combat.Sets == 1);
    thing.Dead = false; thing.Combat = 0; gsi.EntitySetCombatType(script, "COMBAT_SWORD"); CHECK(combat.Sets == 1);
    script.Thing = 0; gsi.EntitySetCombatType(script, "COMBAT_SWORD"); CHECK(combat.Sets == 1);
    thing.Combat = &combat; script.Thing = &thing; gsi.EntitySetCombatType(script, "COMBAT_SWORD");
    CHECK(combat.Sets == 2 && combat.PCombatTypeDef.Object == &g_typeDefs[0] && g_typeDefs[1].Refs == 1 && g_typeDefs[0].Refs == 2);
    if (fails == 0) printf("0089e080_TEST PASS\n");
    return fails;
}

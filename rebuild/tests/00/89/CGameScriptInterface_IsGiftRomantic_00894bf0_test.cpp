// Behaviour test for 00894bf0: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CGameScriptInterface::IsGiftRomantic(const CCharString& defName) const -- true when the named def has a gift sub-def of type 1 (romantic)
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
enum EGiftType { GIFT_0, GIFT_1, GIFT_2 };
class CGiftDef : public CDefClassBase { public: enum { Slot = 7 }; EGiftType Type; };
class CCharString { public: long Index; CCharString(long i) : Index(i) { } };
class CNameLookup { public: long GetDefGlobalIndexFromName(const CCharString& n) const { return n.Index; } };
class CGameScriptInterface { public:
    CNameLookup* DefinitionManager;
    bool IsGiftRomantic(const CCharString& def_name) const;
    bool IsGiftFriendly(const CCharString& def_name) const;
    bool IsGiftOffensive(const CCharString& def_name) const;
};
// ---- row body (verbatim) ----
bool CGameScriptInterface::IsGiftRomantic(const CCharString& def_name) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(def_name);
    CDefPointer<const CThingBaseDef> def;
    if (CGameDefinitionManager::Get().GetDef(index, &def)) {
        CDefPointer<const CGiftDef> gift_def;
        if (def->GetSubDef(&gift_def))
            return gift_def->Type == 1;
    }
    return false;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef d0, d1, d2, plain; CGiftDef g0, g1, g2; g0.Type = GIFT_0; g1.Type = GIFT_1; g2.Type = GIFT_2;
    d0.Sub[7] = &g0; d1.Sub[7] = &g1; d2.Sub[7] = &g2;
    g_manager.Table[1] = &d0; g_manager.Table[2] = &d1; g_manager.Table[3] = &d2; g_manager.Table[4] = &plain;
    CNameLookup names; CGameScriptInterface gsi; gsi.DefinitionManager = &names;
    CHECK(gsi.IsGiftRomantic(CCharString(1)) == (1 == 0) && gsi.IsGiftRomantic(CCharString(2)) == (1 == 1) && gsi.IsGiftRomantic(CCharString(3)) == (1 == 2));
    CHECK(!gsi.IsGiftRomantic(CCharString(4)) && !gsi.IsGiftRomantic(CCharString(9)));
    CHECK(d0.Refs == 1 && d1.Refs == 1 && d2.Refs == 1 && plain.Refs == 1 && g0.Refs == 1 && g1.Refs == 1 && g2.Refs == 1);
    if (fails == 0) printf("00894bf0_TEST PASS\n");
    return fails;
}

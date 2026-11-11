// Behaviour test for 005d9a10: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCInventoryItem::GetHairCardDef(long defIndex) -- static: fetch the thing def for defIndex from the game definition manager and return its CHairCardDef sub-definition as a counted pointer (null when either lookup fails)
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
#ifndef _MSC_VER
#define __fastcall
#endif
class CHairCardDef : public CDefClassBase { public: enum { Slot = 6 }; };
class CTCInventoryItem { public:
    static CDefPointer<const CHairCardDef> __fastcall GetHairCardDef(long def_index);
};
// ---- row body (verbatim) ----
CDefPointer<const CHairCardDef> __fastcall CTCInventoryItem::GetHairCardDef(long def_index)
{
    CDefPointer<const CThingBaseDef> def;
    if (CGameDefinitionManager::Get().GetDef(def_index, &def)) {
        CDefPointer<const CHairCardDef> card_def;
        if (def->GetSubDef(&card_def))
            return card_def;
    }
    return CDefPointer<const CHairCardDef>();
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef with, without; CHairCardDef card; with.Sub[6] = &card; g_manager.Table[3] = &with; g_manager.Table[4] = &without;
    {
        CDefPointer<const CHairCardDef> a = CTCInventoryItem::GetHairCardDef(3);
        CHECK(a.Object == &card && card.Refs == 2 && with.Refs == 1);
        CDefPointer<const CHairCardDef> b = CTCInventoryItem::GetHairCardDef(4);   // no sub-def
        CHECK(b.IsNull() && without.Refs == 1);
        CDefPointer<const CHairCardDef> c = CTCInventoryItem::GetHairCardDef(9);   // no def
        CHECK(c.IsNull() && g_manager.Calls == 3);
    }
    CHECK(card.Refs == 1 && card.Deleted == 0);
    if (fails == 0) printf("005d9a10_TEST PASS\n");
    return fails;
}

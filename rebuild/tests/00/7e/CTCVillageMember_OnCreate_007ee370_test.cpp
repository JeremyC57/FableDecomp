// Behaviour test for 007ee370 CTCVillageMember::OnCreate: the row body verbatim over fixture models of the shared-header interfaces
// (CTCBase::GetDefinitionManager, GetDef<CThingBaseDef>, GetSubDef<CVillageMemberDef>, counted CDefPointer, CThing::GetDefGlobalIndex).
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct Counted { mutable int Refs; Counted() : Refs(1) { } };
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) --Object->Refs; }
    T* operator->() const { return Object; }
    void Set(T* p) { if (Object) --Object->Refs; Object = p; if (p) ++p->Refs; }
};
class CVillageMemberDef : public Counted { };
class CThingBaseDef : public Counted { public:
    const CVillageMemberDef* Sub; mutable int Queries;
    bool GetSubDef(CDefPointer<const CVillageMemberDef>* out) const { ++Queries; out->Set(Sub); return Sub != 0; }
};
class CGameDefinitionManager { public:
    const CThingBaseDef* Def; mutable long Index; mutable int Calls;
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const { ++Calls; Index = index; out->Set(Def); return true; }
};
class CThing { public: unsigned short DefGlobalIndex; unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; } };
class CTCVillageMember { public:
    CThing* ParentThing; const CGameDefinitionManager* Manager; CDefPointer<const CVillageMemberDef> PDef;
    const CGameDefinitionManager& GetDefinitionManager() const { return *Manager; }
    void OnCreate();
};
// ---- row body (verbatim) ----
void CTCVillageMember::OnCreate()
{
    if (ParentThing->GetDefGlobalIndex() != 0) {
        CDefPointer<const CThingBaseDef> def;
        CThing* thing = ParentThing;
        GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
        def->GetSubDef(&PDef);
    }
}
// ---- harness ----
int main()
{
    CVillageMemberDef sub; CThingBaseDef def; def.Sub = &sub; def.Queries = 0;
    CGameDefinitionManager manager; manager.Def = &def; manager.Calls = 0; manager.Index = -1;
    CThing thing; thing.DefGlobalIndex = 0;
    {
        CTCVillageMember member; member.ParentThing = &thing; member.Manager = &manager;
        member.OnCreate();
        CHECK(manager.Calls == 0 && member.PDef.Object == 0);
        thing.DefGlobalIndex = 0x4321;
        member.OnCreate();
        CHECK(manager.Calls == 1 && manager.Index == 0x4321 && member.PDef.Object == &sub && def.Queries == 1);
        CHECK(def.Refs == 1 && sub.Refs == 2);
    }
    CHECK(sub.Refs == 1);
    if (fails == 0) printf("007ee370_TEST PASS\n");
    return fails;
}

// behaviour test for 0x008ECBE0 CTCDegradableBase::OnInitialActivate
#include <cstdio>
struct Def { long RefCount; bool Degradable; bool InitiallyBlocks; int destroyed;
    void AddRef() { ++RefCount; } void Release() { if (--RefCount == 0) ++destroyed; } };
struct DefPtr { Def* Object; DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    DefPtr(Def* d) : Object(d) { } ~DefPtr() { Object->Release(); } Def* operator->() const { return Object; } };
struct Nav { int calls; bool blocking; void SetAsBlockingNavigation(bool b) { ++calls; blocking = b; } };
struct Thing { bool persisted; Nav* nav; float MaxHealth, Health;
    void SetAsGameStatePersisted(bool p) { persisted = p; }
    bool GetInterface(Nav** out, long) { if (!nav) return false; *out = nav; return true; } };
struct Degr {
    Thing* ParentThing; DefPtr PDefinition; Thing* PParentThingPhysical; int healthCalls;
    Degr(Thing* t, Def* d) : ParentThing(t), PDefinition(d), PParentThingPhysical(t), healthCalls(0) { }
    DefPtr GetDefinition() const { return PDefinition; }
    void OnParentHealthChanged(bool) { ++healthCalls; }
    void OnInitialActivate()
    {
        if (GetDefinition()->Degradable) ParentThing->SetAsGameStatePersisted(true);
        if (PParentThingPhysical->Health < PParentThingPhysical->MaxHealth) OnParentHealthChanged(true);
        Nav* navigation;
        if (ParentThing->GetInterface(&navigation, 0x79)) navigation->SetAsBlockingNavigation(PDefinition->InitiallyBlocks);
    }
};
int main()
{
    Def d = { 3, true, true, 0 };
    Nav n = { 0, false };
    Thing t = { false, &n, 10.0f, 5.0f };
    bool ok;
    {
        Degr g(&t, &d);
        g.OnInitialActivate();
        ok = t.persisted && g.healthCalls == 1 && n.calls == 1 && n.blocking && d.RefCount == 3 && d.destroyed == 0;

    }
    d.Degradable = false; t.persisted = false; t.Health = 10.0f; t.nav = 0;
    {
        Degr g(&t, &d);
        g.OnInitialActivate();
        ok = ok && !t.persisted && g.healthCalls == 0 && n.calls == 1;

    }
    if (!ok) { std::printf("008ecbe0_TEST FAIL\n"); return 1; }
    std::printf("008ecbe0_TEST PASS\n");
    return 0;
}
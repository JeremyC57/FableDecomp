#include <cstdio>
struct CBaseObject { int id; };
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CNavigationInput {
    C3DVector Destination; float Proximity; long MovementType; float Speed;
    CNavigationInput() { }
    CNavigationInput(const CNavigationInput& o) : Destination(o.Destination), Proximity(o.Proximity), MovementType(o.MovementType), Speed(o.Speed) { }
};
struct CCountedPointerAction {
    void* Object; long* PCount;
    CCountedPointerAction() : Object(0), PCount(0) { }
    CCountedPointerAction(const CCountedPointerAction& o) : Object(o.Object), PCount(o.PCount) { if (PCount) ++*PCount; }
};
static int g_sets = 0;
struct CBaseIntelligentPointer {
    CBaseObject* m_item;
    virtual ~CBaseIntelligentPointer() { }
    CBaseIntelligentPointer() : m_item(0) { }
    CBaseObject* GetPItem() const { return m_item; }
    void SetPItem(CBaseObject* item) { ++g_sets; m_item = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(const CIntelligentPointerThing& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
struct CBrainOutput {
    unsigned long Types; long LookFlags; CNavigationInput NavigationInput; CCountedPointerAction PAction;
    C3DVector MoveToPos, LookAtPos; CIntelligentPointerThing LookAtThing;
    CBrainOutput() { }
    CBrainOutput(const CBrainOutput& other)
        : Types(other.Types), LookFlags(other.LookFlags), NavigationInput(other.NavigationInput),
          PAction(other.PAction), MoveToPos(other.MoveToPos), LookAtPos(other.LookAtPos), LookAtThing(other.LookAtThing) { }
};
int main()
{
    CBaseObject thing; thing.id = 5; long count = 1; int action = 0;
    CBrainOutput a; a.Types = 3; a.LookFlags = 4; a.NavigationInput.Destination.X = 1; a.NavigationInput.Proximity = 2;
    a.NavigationInput.MovementType = 6; a.NavigationInput.Speed = 7; a.PAction.Object = &action; a.PAction.PCount = &count;
    a.MoveToPos.Y = 8; a.LookAtPos.Z = 9; a.LookAtThing.SetPItem(&thing);
    CBrainOutput b(a);
    if (b.Types != 3 || b.LookFlags != 4 || b.NavigationInput.Destination.X != 1 || b.NavigationInput.MovementType != 6 ||
        b.NavigationInput.Speed != 7 || b.PAction.Object != &action || count != 2 || b.MoveToPos.Y != 8 || b.LookAtPos.Z != 9 ||
        b.LookAtThing.GetPItem() != &thing || g_sets != 2)
    { std::printf("006d6110_TEST FAIL\n"); return 1; }
    a.PAction.PCount = 0; CBrainOutput c(a);
    if (count != 2 || c.PAction.PCount != 0) { std::printf("006d6110_TEST FAIL\n"); return 1; }
    std::printf("006d6110_TEST PASS\n");
    return 0;
}
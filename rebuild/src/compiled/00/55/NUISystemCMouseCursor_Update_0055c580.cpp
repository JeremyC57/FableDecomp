// NUISystem::CMouseCursor::Update(float deltaTime) -- the state-component update, then publish the cursor position to the shared static
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C580, 35 bytes (`ret 4`).
// Callee: NUISystem::CChangingStateComponent::Update(float) @ 0x0052C7E0 (landed).
// Label: CMouseCursor vtable 0x0124C22C slot 1 (rtti-vtables.tsv); Ego_r.pdb names ?Update@CMouseCursor@NUISystem@@UAEXM@Z.
// The static C2DVector at 0x013B8AD8 is only touched here and in Initialise (0x0055C560, which copies it back into
// Position); the name is descriptive.
// Layout: CComponent::Position at retail +0x34 (as the landed ChangePosition 0x0052E9C0).
// Shape: a memberwise C2DVector copy into the static through ecx / edx.
class C2DVector { public:
    float X;
    float Y;
};

namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000();
    virtual void Update(float deltaTime);         // slot 1; 0x0052C7E0
    unsigned char _pad04[0x34 - 4];
    C2DVector Position;                           // +0x34
};

class CMouseCursor : public CChangingStateComponent {
public:
    static C2DVector GCursorPosition;             // 0x013B8AD8
    virtual void Update(float deltaTime);
};

void CMouseCursor::Update(float deltaTime)
{
    CChangingStateComponent::Update(deltaTime);
    GCursorPosition = Position;
}
}
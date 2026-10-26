#include <cstdio>
class C2DVector { public: float X; float Y; };
static float g_baseDt = 0.0f;
struct CChangingStateComponent {
    C2DVector Position;
    virtual ~CChangingStateComponent() {}
    virtual void Update(float dt) { g_baseDt = dt; Position.X += 1.0f; }
};
struct CMouseCursor : CChangingStateComponent {
    static C2DVector GCursorPosition;
    virtual void Update(float dt) { CChangingStateComponent::Update(dt); GCursorPosition = Position; }
};
C2DVector CMouseCursor::GCursorPosition = { 0.0f, 0.0f };
int main()
{
    CMouseCursor m; m.Position.X = 10.0f; m.Position.Y = 20.0f;
    m.Update(0.25f);
    if (g_baseDt != 0.25f || CMouseCursor::GCursorPosition.X != 11.0f || CMouseCursor::GCursorPosition.Y != 20.0f)
        { std::printf("0055c580_TEST FAIL\n"); return 1; }
    std::printf("0055c580_TEST PASS\n");
    return 0;
}
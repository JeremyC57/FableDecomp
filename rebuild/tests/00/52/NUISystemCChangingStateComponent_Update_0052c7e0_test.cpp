#include <cstdio>
struct CUIState { unsigned char Flags; };
class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; };
static float g_baseDt = 0.0f; static int g_settled = 0;
struct CComponent {
    C2DVector InitialPosition, InitialZoom; CRGBColour TargetColour;
    virtual ~CComponent() { }
    virtual bool UpdateStates() = 0;
    virtual const CUIState* GetState(unsigned long state) = 0;
    virtual void OnStatesSettled() = 0;
    void Update(float dt) { g_baseDt = dt; }
};
struct CChangingStateComponent : CComponent {
    unsigned long TargetState; bool PreviousUpdateChanged;
    bool m_changed; CUIState m_state; bool m_hasState; unsigned long m_asked;
    virtual bool UpdateStates() { return m_changed; }
    virtual const CUIState* GetState(unsigned long s) { m_asked = s; return m_hasState ? &m_state : 0; }
    virtual void OnStatesSettled() { ++g_settled; }
    void Update(float deltaTime)
    {
        PreviousUpdateChanged = UpdateStates();
        const CUIState* state = GetState(TargetState);
        if (state) {
            if (state->Flags & 0x10) { TargetColour.B = 0xff; TargetColour.G = 0xff; TargetColour.R = 0xff; TargetColour.A = 0xff; }
            if (state->Flags & 0x20) { InitialPosition.Y = 0.0f; InitialPosition.X = 0.0f; }
            if (state->Flags & 0x40) InitialZoom.X = InitialZoom.Y = 1.0f;
        }
        CComponent::Update(deltaTime);
        if (!PreviousUpdateChanged) OnStatesSettled();
    }
};
int main()
{
    CChangingStateComponent c;
    c.InitialPosition.X = 3; c.InitialPosition.Y = 4; c.InitialZoom.X = 2; c.InitialZoom.Y = 2;
    c.TargetColour.B = c.TargetColour.G = c.TargetColour.R = c.TargetColour.A = 0;
    c.TargetState = 7; c.m_changed = true; c.m_hasState = true; c.m_state.Flags = 0x30;
    c.Update(0.25f);
    if (c.m_asked != 7 || !c.PreviousUpdateChanged || c.TargetColour.A != 0xff || c.InitialPosition.X != 0 ||
        c.InitialPosition.Y != 0 || c.InitialZoom.X != 2 || g_baseDt != 0.25f || g_settled != 0)
    { std::printf("0052c7e0_TEST FAIL\n"); return 1; }
    c.m_changed = false; c.m_state.Flags = 0x40;
    c.Update(0.5f);
    if (c.InitialZoom.X != 1 || c.InitialZoom.Y != 1 || g_settled != 1) { std::printf("0052c7e0_TEST FAIL\n"); return 1; }
    c.m_hasState = false; c.InitialZoom.X = 5; c.Update(0.5f);
    if (c.InitialZoom.X != 5 || g_settled != 2) { std::printf("0052c7e0_TEST FAIL\n"); return 1; }
    std::printf("0052c7e0_TEST PASS\n");
    return 0;
}
// Behaviour test for NUISystem::CChangingStateComponent::ProcessChangeState (0x0052C920): a copy of the model with recording
// stand-ins for the four virtual callees; checks the early out, the three flag arms, the non-zero tests and the linear flag.
#include <cstdio>
static const float DELTA_NOTIONAL_ZERO = 1e-4f;
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
    bool IsNonZero() const { return GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
};
class CRGBColour { public:
    union { struct { unsigned char B, G, R, A; }; unsigned long IntValue; };
    CRGBColour() { }
    void Initialise(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& c) { Initialise(c.R, c.G, c.B, c.A); }
};
struct CUIState {
    unsigned long StateChangeFlag; long StateChangeType; C2DVector Position; C2DVector Zoom; CRGBColour Colour;
    float UpdateTime; bool LinearChange;
};
struct Call { int kind; float x, y; unsigned long colour; float time; bool linear; };
struct Component {
    C2DVector Position, Zoom; CRGBColour Colour;
    unsigned long CurrentState, TargetState; float UpdateTime;
    CUIState States[2]; int GetStateCalls; unsigned long GetStateArgs[4]; Call Calls[8]; int CallCount;
    CUIState* GetState(unsigned long s) { GetStateArgs[GetStateCalls++] = s; return &States[s]; }
    void ChangePositionDelta(const C2DVector& d, float t, bool l) { Call c = { 1, d.X, d.Y, 0, t, l }; Calls[CallCount++] = c; }
    void ChangeZoomDelta(const C2DVector& d, float t, bool l) { Call c = { 2, d.X, d.Y, 0, t, l }; Calls[CallCount++] = c; }
    void ChangeColourDelta(const CRGBColour& d, float t, bool l) { Call c = { 3, 0, 0, d.IntValue, t, l }; Calls[CallCount++] = c; }
    void ProcessChangeState()
    {
        if (TargetState == CurrentState)
            return;
        CUIState* target = GetState(TargetState);
        CUIState* current = GetState(CurrentState);
        (void)current;
        if (target->StateChangeFlag & 1) {
            if ((target->Position - Position).IsNonZero())
                if (target->LinearChange)
                    ChangePositionDelta(target->Position - Position, UpdateTime, true);
                else
                    ChangePositionDelta(target->Position - Position, UpdateTime, false);
        }
        if (target->StateChangeFlag & 4) {
            if ((target->Zoom - Zoom).IsNonZero())
                if (target->LinearChange)
                    ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, true);
                else
                    ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, false);
        }
        if (target->StateChangeFlag & 2) {
            CRGBColour delta = target->Colour;
            delta.R -= Colour.R;
            delta.G -= Colour.G;
            delta.B -= Colour.B;
            delta.A -= Colour.A;
            if (delta.IntValue != 0)
                if (target->LinearChange)
                    ChangeColourDelta(delta, UpdateTime, true);
                else
                    ChangeColourDelta(delta, UpdateTime, false);
        }
    }
};
static void Reset(Component& c, unsigned long flags, bool linear)
{
    c.Position = C2DVector(10.0f, 20.0f); c.Zoom = C2DVector(1.0f, 1.0f); c.Colour.Initialise(10, 20, 30, 40);
    c.CurrentState = 0; c.TargetState = 1; c.UpdateTime = 0.5f; c.GetStateCalls = 0; c.CallCount = 0;
    c.States[1].StateChangeFlag = flags; c.States[1].Position = C2DVector(13.0f, 16.0f); c.States[1].Zoom = C2DVector(2.0f, 3.0f);
    c.States[1].Colour.Initialise(15, 10, 30, 255); c.States[1].LinearChange = linear;
    c.States[0] = c.States[1];
}
int main()
{
    Component c; bool ok = true;
    // No pending change: nothing is called at all.
    Reset(c, 7, true); c.TargetState = 0; c.ProcessChangeState();
    ok = ok && c.GetStateCalls == 0 && c.CallCount == 0;
    // All three arms, linear: target state fetched first, then the current one; position, zoom, colour in that order.
    Reset(c, 7, true); c.ProcessChangeState();
    ok = ok && c.GetStateCalls == 2 && c.GetStateArgs[0] == 1 && c.GetStateArgs[1] == 0 && c.CallCount == 3;
    ok = ok && c.Calls[0].kind == 1 && c.Calls[0].x == 3.0f && c.Calls[0].y == -4.0f && c.Calls[0].time == 0.5f && c.Calls[0].linear;
    ok = ok && c.Calls[1].kind == 2 && c.Calls[1].x == 1.0f && c.Calls[1].y == 2.0f && c.Calls[1].linear;
    CRGBColour expect; expect.Initialise(5, (unsigned char)(10 - 20), 0, (unsigned char)(255 - 40));
    ok = ok && c.Calls[2].kind == 3 && c.Calls[2].colour == expect.IntValue && c.Calls[2].time == 0.5f && c.Calls[2].linear;
    // Non-linear, colour only.
    Reset(c, 2, false); c.ProcessChangeState();
    ok = ok && c.CallCount == 1 && c.Calls[0].kind == 3 && !c.Calls[0].linear && c.Calls[0].colour == expect.IntValue;
    // Position + zoom, non-linear; the colour arm is masked off.
    Reset(c, 5, false); c.ProcessChangeState();
    ok = ok && c.CallCount == 2 && c.Calls[0].kind == 1 && !c.Calls[0].linear && c.Calls[1].kind == 2 && !c.Calls[1].linear;
    // Deltas that are (notionally) zero start nothing: position within the epsilon, equal zoom, equal colour.
    Reset(c, 7, true);
    c.States[1].Position = C2DVector(10.00001f, 20.0f); c.States[1].Zoom = c.Zoom; c.States[1].Colour = c.Colour;
    c.ProcessChangeState();
    ok = ok && c.GetStateCalls == 2 && c.CallCount == 0;
    // One differing channel (alpha only) is enough for the colour arm.
    Reset(c, 2, true); c.States[1].Colour.Initialise(10, 20, 30, 41); c.ProcessChangeState();
    ok = ok && c.CallCount == 1 && c.Calls[0].colour == 0x01000000UL;
    if (!ok) { std::printf("0052c920_TEST FAIL\n"); return 1; }
    std::printf("0052c920_TEST PASS\n");
    return 0;
}
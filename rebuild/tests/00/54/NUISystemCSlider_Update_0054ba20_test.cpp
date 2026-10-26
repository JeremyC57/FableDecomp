#include <cstdio>
#include <math.h>
class C2DVector { public:
    float X, Y;
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
struct CUIState {
    unsigned long StateChangeFlag;   // +0x00
    long          StateChangeType;   // +0x04
    C2DVector     Position;          // +0x08
};
struct CStateMap {                   // STLport map<unsigned long, CUIState> (12 bytes)
    void* Header; unsigned long NodeCount; unsigned long Compare;
    CUIState Entry; unsigned long LastKey;
    CUIState& operator[](const unsigned long& key) { LastKey = key; return Entry; }
};
struct CChangingStateComponent {
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear)
    { LastDelta = delta; LastTime = time; LastLinear = linear; ++Calls; }
    C2DVector LastDelta; float LastTime; bool LastLinear; int Calls; float UpdatedWith;
    CStateMap     States;            // +0x24
    float         Time;              // +0x30
    C2DVector     Position;          // +0x34
    C2DVector     TargetPosition;    // +0x3c
    void Update(float dt) { UpdatedWith = dt; }
};
struct CSlider : CChangingStateComponent {
    C2DVector Dimensions;            // +0x184
    C2DVector Min;                   // +0x18c
    C2DVector Max;                   // +0x194
    C2DVector Step;                  // +0x19c
    C2DVector Value;                 // +0x1a4
    bool      Move;                  // +0x1ac
    void Update(float dt);
};

static inline float SliderPixels(float value, float minimum, float dimension, float range)
{
    float offset = (value - minimum) * dimension;
    return offset / range;
}
void CSlider::Update(float dt)
{
    float rangeX = Max.X - Min.X;
    if ((rangeX != 0.0f && fabs(SliderPixels(Value.X, Min.X, Dimensions.X, rangeX) - TargetPosition.X) >= 1.0f) ||
        (Max.Y - Min.Y != 0.0f && fabs(SliderPixels(Value.Y, Min.Y, Dimensions.Y, Max.Y - Min.Y) - TargetPosition.Y) >= 1.0f))
        Move = true;
    if (Move) {
        C2DVector delta;
        if (rangeX != 0.0f)
            delta.X = SliderPixels(Value.X, Min.X, Dimensions.X, rangeX) - Position.X;
        else
            delta.X = 0.0f;
        float rangeY = Max.Y - Min.Y;
        if (rangeY != 0.0f)
            delta.Y = SliderPixels(Value.Y, Min.Y, Dimensions.Y, rangeY) - Position.Y;
        else
            delta.Y = 0.0f;
        ChangePositionDelta(delta, 0.2f, false);
        unsigned long key = 0;
        States[key].Position += delta;
        Move = false;
    }
    CChangingStateComponent::Update(dt);
}

static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    static CSlider s;
    s.Dimensions.X = 100; s.Dimensions.Y = 50; s.Min.X = 0; s.Min.Y = 0; s.Max.X = 10; s.Max.Y = 0;
    s.Value.X = 5; s.Value.Y = 3; s.Position.X = 20; s.Position.Y = 7; s.TargetPosition.X = 50.5f; s.TargetPosition.Y = 0;
    s.States.Entry.Position.X = 1; s.States.Entry.Position.Y = 2; s.States.LastKey = 99;
    s.Move = false; s.Calls = 0;
    s.Update(0.25f);                       // |50 - 50.5| < 1 and Y range 0: no move
    bool ok = s.Calls == 0 && !s.Move && s.UpdatedWith == 0.25f;
    s.TargetPosition.X = 40;               // 10 px off: move by (50 - 20, 0)
    s.Update(0.5f);
    ok = ok && s.Calls == 1 && near(s.LastDelta.X, 30) && near(s.LastDelta.Y, 0) && near(s.LastTime, 0.2f) && !s.LastLinear
            && s.States.LastKey == 0 && near(s.States.Entry.Position.X, 31) && near(s.States.Entry.Position.Y, 2)
            && !s.Move && s.UpdatedWith == 0.5f;
    s.TargetPosition.X = 50; s.Max.Y = 6; s.TargetPosition.Y = 0;   // Y: 3/6*50 = 25 px off
    s.Update(1.0f);
    ok = ok && s.Calls == 2 && near(s.LastDelta.X, 30) && near(s.LastDelta.Y, 18) && near(s.States.Entry.Position.Y, 20);
    s.Max.X = 0; s.Max.Y = 0; s.Move = true;                         // forced move with zero ranges: zero delta
    s.Update(1.0f);
    ok = ok && s.Calls == 3 && s.LastDelta.X == 0 && s.LastDelta.Y == 0 && !s.Move;
    if (!ok) { std::printf("0054ba20_TEST FAIL\n"); return 1; }
    std::printf("0054ba20_TEST PASS\n");
    return 0;
}
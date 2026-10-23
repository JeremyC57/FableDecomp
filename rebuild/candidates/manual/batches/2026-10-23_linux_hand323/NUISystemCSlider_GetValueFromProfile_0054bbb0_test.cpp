// Behaviour test for NUISystem::CSlider::GetValueFromProfile (0x0054BBB0): the value copies, the per-setting maximum and
// the thumb delta ((value - min) * dimension / range - position, 0 on an empty range) added to the default state's position.
#include <cstdio>
#include <cmath>

struct Vec2 { float X, Y; };
struct Profile {
    bool useNew;
    float shadow, shadowNew, sound, texture, maxShadow, maxTexture;
    float GetShadowDetail() const { return useNew ? shadowNew : shadow; }
};
struct Slider {
    Vec2 Dimensions, Min, Max, Value, OldValue, StartValue, Position, StatePos, LastDelta;
    long PDefAction;
    int moves;
    void ChangePositionDelta(const Vec2& d) { LastDelta = d; ++moves; }
    static float Pixels(float v, float mn, float dim, float range) { float o = (v - mn) * dim; return o / range; }
    void Thumb()
    {
        Vec2 delta;
        float rx = Max.X - Min.X;
        delta.X = rx != 0.0f ? Pixels(Value.X, Min.X, Dimensions.X, rx) - Position.X : 0.0f;
        float ry = Max.Y - Min.Y;
        delta.Y = ry != 0.0f ? Pixels(Value.Y, Min.Y, Dimensions.Y, ry) - Position.Y : 0.0f;
        ChangePositionDelta(delta);
        StatePos.X += delta.X;
        StatePos.Y += delta.Y;
    }
    void GetValueFromProfile(const Profile& p)
    {
        if (PDefAction == 0) return;
        switch (PDefAction) {
        case 0x29: Value.X = OldValue.X = StartValue.X = p.sound; Thumb(); break;
        case 0x103: Value.X = OldValue.X = StartValue.X = p.GetShadowDetail(); Max.X = p.maxShadow; Thumb(); break;
        case 0x10a: Value.X = OldValue.X = StartValue.X = p.texture; Max.X = p.maxTexture; Thumb(); break;
        }
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    bool bad = false;
    Profile p = { true, 0.25f, 0.75f, 0.5f, 2.0f, 1.0f, 4.0f };
    Slider s;
    s.Dimensions.X = 100.0f; s.Dimensions.Y = 10.0f;
    s.Min.X = 0.0f; s.Min.Y = 0.0f; s.Max.X = 1.0f; s.Max.Y = 0.0f;
    s.Value.X = s.Value.Y = 0.0f; s.Position.X = 5.0f; s.Position.Y = 3.0f;
    s.StatePos.X = s.StatePos.Y = 0.0f; s.moves = 0;
    s.PDefAction = 0x29;
    s.GetValueFromProfile(p);
    if (!Near(s.Value.X, 0.5f) || !Near(s.OldValue.X, 0.5f) || !Near(s.StartValue.X, 0.5f)) bad = true;
    if (!Near(s.LastDelta.X, 45.0f) || !Near(s.LastDelta.Y, 0.0f) || !Near(s.StatePos.X, 45.0f)) bad = true;
    s.PDefAction = 0x103;
    s.GetValueFromProfile(p);
    if (!Near(s.Value.X, 0.75f) || !Near(s.Max.X, 1.0f) || !Near(s.LastDelta.X, 70.0f)) bad = true;
    s.PDefAction = 0x10a;
    s.GetValueFromProfile(p);
    if (!Near(s.Max.X, 4.0f) || !Near(s.LastDelta.X, 45.0f) || s.moves != 3) bad = true;
    s.PDefAction = 0;
    s.GetValueFromProfile(p);
    if (s.moves != 3) bad = true;
    s.PDefAction = 0x77;
    s.GetValueFromProfile(p);
    if (s.moves != 3) bad = true;
    if (bad) { std::printf("0054bbb0_TEST FAIL\n"); return 1; }
    std::printf("0054bbb0_TEST PASS\n");
    return 0;
}

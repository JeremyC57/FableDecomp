#include <cstdio>
// Behaviour copy of CCreatureAction_ReleaseStrengthBerserk::FrameUpdate (0x008E6500): clamp the four super-rumble values and rumble joystick type 4, looping.
struct Params { float a, b, c, d; bool looping; };
struct Joystick { long type; Params p; long flag; int calls; void Rumble(long t, const Params& pr, long f) { type = t; p = pr; flag = f; ++calls; } };
struct Berserk { float v[4]; void GetSuperRumble(float& a, float& b, float& c, float& d) { a = v[0]; b = v[1]; c = v[2]; d = v[3]; } };
static void Clamp01(float& v) { if (v < 0.0f) v = 0.0f; else if (v > 1.0f) v = 1.0f; }
struct Action {
    bool baseUpdated; Berserk* berserk; Joystick* joy;
    void FrameUpdate() {
        baseUpdated = true;
        if (berserk) {
            float a, b, c, d; berserk->GetSuperRumble(a, b, c, d);
            Clamp01(a); Clamp01(b); Clamp01(c); Clamp01(d);
            Params p = { a, b, c, d, true };
            joy->Rumble(4, p, 0);
        }
    }
};
int main()
{
    Berserk b = { { -0.5f, 0.25f, 1.5f, 1.0f } }; Joystick j = { 0, { 0, 0, 0, 0, false }, 1, 0 };
    Action a = { false, &b, &j };
    a.FrameUpdate();
    bool ok = a.baseUpdated && j.calls == 1 && j.type == 4 && j.flag == 0 && j.p.a == 0.0f && j.p.b == 0.25f && j.p.c == 1.0f && j.p.d == 1.0f && j.p.looping;
    a.berserk = 0; a.FrameUpdate();
    ok = ok && j.calls == 1;
    std::printf(ok ? "008e6500_TEST PASS\n" : "008e6500_TEST FAIL\n");
    return ok ? 0 : 1;
}

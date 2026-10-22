#include <cstdio>
#include <cmath>
// Behaviour model of CInputProcessControlCreatureRightStick::ProcessInput (0x0068D890): stick direction classification.
struct State { bool up, down, look; int pushUp, pushDown, lookStarts, lookEnds; };
static void Stick(State& s, float x, float y, bool mouse)
{
    y *= -1.0f;
    float dead = 0.2f;
    float mag = (float)std::sqrt(x * x + y * y);
    float dx = 0.0f, dy = 0.0f;
    if (mag > dead) { dx = x; dy = y; }
    float c10 = (float)std::cos(3.1415927f / 180.0 * 10.0), c80 = (float)std::cos(3.1415927f / 180.0 * 80.0);
    float upc = mag > 0.0001f ? dy / mag : 0.0f;
    float rc = mag > 0.0001f ? dx / mag : 0.0f;
    if (upc < 0.0f && std::fabs(upc) > c10) {
        if (mouse) { s.look = false; return; }
        if (mag > 0.8f) { if (!s.up) { ++s.pushUp; s.look = mouse; s.up = true; } return; }
        s.up = false; return;
    }
    if (upc > 0.0f && std::fabs(upc) > c10) {
        if (mouse) { s.look = false; return; }
        if (mag > 0.8f) { if (!s.down) { ++s.pushDown; s.look = mouse; s.down = true; } return; }
        s.down = false; return;
    }
    if (std::fabs(rc) > c80 && mag > dead) { if (!s.look) { ++s.lookStarts; s.look = true; } return; }
    if (s.look && mag > dead) return;
    if (s.look) { ++s.lookEnds; s.look = false; return; }
    s.down = false; s.up = false;
}
int main()
{
    State s = { false, false, false, 0, 0, 0, 0 };
    Stick(s, 0.0f, 1.0f, false); Stick(s, 0.0f, 1.0f, false);       // held: one push
    int ok = s.pushUp == 1 && s.up;
    Stick(s, 0.0f, 0.0f, false); ok = ok && !s.up;                     // released
    Stick(s, 0.0f, -1.0f, false); ok = ok && s.pushDown == 1 && s.down;
    Stick(s, 1.0f, 0.0f, false); ok = ok && s.lookStarts == 1 && s.look;
    Stick(s, 0.0f, 0.0f, false); ok = ok && s.lookEnds == 1 && !s.look;
    Stick(s, 0.0f, 1.0f, true); ok = ok && s.pushUp == 1;               // mouse: no list step
    if (!ok) { std::printf("0068d890_TEST FAIL\n"); return 1; }
    std::printf("0068d890_TEST PASS\n");
    return 0;
}

// Behaviour test for NPlayerGui::CDrawPerceivers::Draw (0x0064499C): the frame interpolation eases toward the state and clamps,
// the throb scale grows while animating and relaxes to 1 when settled, the back graphic follows the allegiance, the eye frame is
// interpolation * 0.5 across the frame list, and the count text is capped at 99 and only drawn for a positive count.
#include <cstdio>

struct Perceivers { long state, allegiance; float interp, throb; long count; };
struct Def { float fade, throbTime, maxThrob; long friendly, neutral, enemy; long frames; };
struct Out { bool drawn; long back; long frame; long shown; bool text; };

Out Draw(Perceivers& p, const Def& d, bool show, unsigned char alpha, float dt)
{
    Out o; o.drawn = false; o.text = false;
    if (!show || alpha == 0)
        return o;
    float target = (float)p.state;
    if (p.interp != target) {
        if (p.interp < target) {
            p.interp += dt / (d.fade * 0.5f);
            if (p.interp > target) p.interp = target;
        } else if (p.interp > target) {
            p.interp -= dt / (d.fade * 0.5f);
            if (p.interp < target) p.interp = target;
        }
        if (p.throb < d.maxThrob) {
            p.throb += dt / d.throbTime;
            if (p.throb > d.maxThrob) p.throb = d.maxThrob;
        }
    } else if (p.throb > 1.0f) {
        p.throb -= dt / d.throbTime;
        if (p.throb < 1.0f) p.throb = 1.0f;
    }
    o.drawn = true;
    o.frame = (long)(((float)d.frames - 1.0f) * (p.interp * 0.5f));
    o.back = d.neutral;
    if (p.allegiance == 1) o.back = d.friendly;
    else if (p.allegiance == 2) o.back = d.enemy;
    if (p.count > 0) { o.text = true; o.shown = p.count > 99 ? 99 : p.count; }
    return o;
}

int main()
{
    int ok = 1;
    Def d; d.fade = 1.0f; d.throbTime = 2.0f; d.maxThrob = 1.5f; d.friendly = 10; d.neutral = 11; d.enemy = 12; d.frames = 5;
    Perceivers p; p.state = 2; p.allegiance = 2; p.interp = 0.0f; p.throb = 1.0f; p.count = 150;
    Out o = Draw(p, d, true, 0, 0.25f);
    if (o.drawn || p.interp != 0.0f) ok = 0;
    o = Draw(p, d, true, 200, 0.25f);
    if (!o.drawn || p.interp != 0.5f || p.throb != 1.125f || o.back != 12 || !o.text || o.shown != 99 || o.frame != 1) ok = 0;
    for (int i = 0; i < 10; ++i) o = Draw(p, d, true, 200, 0.25f);
    if (p.interp != 2.0f || o.frame != 4) ok = 0;
    if (p.throb != 1.0f) ok = 0;   // settled: the throb relaxed back to 1
    p.state = 0; p.allegiance = 1; p.count = 0;
    o = Draw(p, d, true, 200, 0.25f);
    if (p.interp != 1.5f || o.back != 10 || o.text) ok = 0;
    std::printf(ok ? "0064499c_TEST PASS\n" : "0064499c_TEST FAIL\n");
    return ok ? 0 : 1;
}
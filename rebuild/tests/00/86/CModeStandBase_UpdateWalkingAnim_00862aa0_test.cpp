// Behaviour test for CModeStandBase::UpdateWalkingAnim 0x00862AA0: model of the band selection (run->sprint when sprinting
// past running speed, jog->run, walk->jog, stand->walk; walk->run / stand->walk without a jog anim), the movement counter, and
// the existing-anim status tables recovered from the retail jump tables / compare chains.
#include <cstdio>
#include <cmath>

enum Status { NONE, SAME, REPLACE, OTHER };
struct Setup { float speed; int type; int anim; };
static Setup MkSetup(float speed, int type, int anim) { Setup r; r.speed = speed; r.type = type; r.anim = anim; return r; }
struct Call { Status status; Setup from, to; int calls; };
static const float EPS = 1e-4f;
struct Speeds { float walk, jog, run, sprint; };
struct Mode {
    bool hasWalk, hasJog, hasRun; int frames; Call last;
    Mode() : hasWalk(true), hasJog(true), hasRun(true), frames(0) { last.calls = 0; }
    void Set(Status st, Setup f, Setup t) { last.status = st; last.from = f; last.to = t; ++last.calls; }
    void Update(float vx, float vy, bool sprinting, int type, const Speeds& s)
    {
        if (!hasWalk || !hasRun) return;
        float speed = (float)std::sqrt(vx * vx + vy * vy);
        if (speed > EPS) frames = 2; else --frames;
        if (sprinting && s.run + EPS < speed) {
            Set(type == 2 ? SAME : REPLACE, MkSetup(s.run, 3, 'R'), MkSetup(s.sprint, 2, 'S')); return;
        }
        Status st;
        if (hasJog) {
            if (s.jog + EPS < speed) {
                st = (type == 4 || type == 5) ? REPLACE : type == 3 ? SAME : type == 2 ? NONE : OTHER;
                Set(st, MkSetup(s.jog, 4, 'J'), MkSetup(s.run, 3, 'R'));
            } else if (s.walk + EPS < speed) {
                st = type == 5 ? REPLACE : type == 4 ? SAME : (type == 2 || type == 3) ? NONE : OTHER;
                Set(st, MkSetup(s.walk, 5, 'W'), MkSetup(s.jog, 4, 'J'));
            } else {
                st = (type >= 2 && type <= 4) ? NONE : type == 5 ? SAME : OTHER;
                Set(st, MkSetup(0.0f, 5, 'T'), MkSetup(s.walk, 5, 'W'));
            }
        } else if (s.walk + EPS < speed) {
            st = type == 2 ? NONE : type == 3 ? SAME : type == 5 ? REPLACE : OTHER;
            Set(st, MkSetup(s.walk, 5, 'W'), MkSetup(s.run, 3, 'R'));
        } else {
            st = (type == 2 || type == 3) ? NONE : type == 5 ? SAME : OTHER;
            Set(st, MkSetup(0.0f, 5, 'T'), MkSetup(s.walk, 5, 'W'));
        }
    }
};
int main()
{
    bool ok = true;
    Speeds s = { 1.0f, 2.0f, 3.0f, 4.0f };
    Mode m;
    m.Update(3.0f, 4.0f, true, 2, s);   // speed 5 > run: sprint pair, type 2 -> SAME
    ok = ok && m.frames == 2 && m.last.from.anim == 'R' && m.last.to.anim == 'S' && m.last.status == SAME && m.last.to.type == 2;
    m.Update(3.0f, 4.0f, false, 4, s);  // not sprinting: jog->run, type 4 -> REPLACE
    ok = ok && m.last.from.anim == 'J' && m.last.to.anim == 'R' && m.last.status == REPLACE;
    m.Update(1.5f, 0.0f, false, 4, s);  // walk->jog, type 4 -> SAME
    ok = ok && m.last.from.anim == 'W' && m.last.to.anim == 'J' && m.last.status == SAME;
    m.Update(0.0f, 0.0f, false, 4, s);  // stand->walk, counter counts down, type 4 -> NONE
    ok = ok && m.frames == 1 && m.last.from.anim == 'T' && m.last.from.speed == 0.0f && m.last.status == NONE;
    m.hasJog = false;
    m.Update(1.5f, 0.0f, false, 5, s);  // no jog: walk->run, type 5 -> REPLACE
    ok = ok && m.last.from.anim == 'W' && m.last.to.anim == 'R' && m.last.status == REPLACE;
    m.Update(0.5f, 0.0f, false, 4, s);  // no jog, slow: type 4 -> OTHER
    ok = ok && m.last.from.anim == 'T' && m.last.status == OTHER;
    int calls = m.last.calls; m.hasRun = false; m.Update(9.0f, 0.0f, true, 2, s);
    ok = ok && m.last.calls == calls;
    if (ok) std::printf("00862aa0_TEST PASS\n"); else std::printf("00862aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
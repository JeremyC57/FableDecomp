// Behaviour test for 0x007534B0 CTCLook::IsPosInViewRange: a g++ model of the matched source (retail semantics:
// false when the parent creature is null or dead, false while its animation is in the blocking state 7 with queued
// entries and flag bit 1 clear; otherwise the wrapped look angle must be within half the view angle of the target).
#include <cstdio>
#include <cmath>
struct Creature { bool dead; };
struct Anim { unsigned char state, flags; int start, finish; };
static float Diff(float a, float b) { float d = a - b; d = d - std::floor(d); return d > 0.5f ? d - 1.0f : d; }
static bool InView(const Creature* creature, const Anim& anim, float body, float head, float eyes, bool fromHead, bool withEyes, float toPos, float viewAngle)
{
    if (creature == 0) return false;
    if (creature->dead) return false;
    if (anim.state == 7 && anim.start != anim.finish && !(anim.flags & 2)) return false;
    float look = body;
    if (fromHead) look += head;
    if (withEyes) look += eyes;
    float angle = look;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)std::fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return std::fabs(Diff(angle, toPos)) * 2.0f <= viewAngle;
}
int main() {
    Creature alive = { false }, dead = { true };
    Anim idle = { 0, 0, 0, 0 }, blocking = { 7, 0, 0, 1 }, unblocked = { 7, 2, 0, 1 }, empty = { 7, 0, 0, 0 };
    int ok = InView(&alive, idle, 0.1f, 0.0f, 0.0f, false, false, 0.15f, 0.25f)
        && !InView(&alive, idle, 0.1f, 0.0f, 0.0f, false, false, 0.4f, 0.25f)
        && InView(&alive, idle, 0.1f, 0.25f, 0.0f, true, false, 0.4f, 0.25f)
        && InView(&alive, idle, 0.9f, 0.25f, 0.1f, true, true, 0.3f, 0.25f)
        && InView(&alive, idle, -0.2f, 0.0f, 0.0f, false, false, 0.8f, 0.1f)
        && !InView(0, idle, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f)
        && !InView(&dead, idle, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f)
        && !InView(&alive, blocking, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f)
        && InView(&alive, unblocked, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f)
        && InView(&alive, empty, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f);
    std::printf(ok ? "007534b0_TEST PASS\n" : "007534b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
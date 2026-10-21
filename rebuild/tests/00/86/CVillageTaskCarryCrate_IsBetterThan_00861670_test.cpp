// Behaviour test for 0x00861670 (CVillageTaskCarryCrate::IsBetterThan): validity gate, priority ordering, nearer crate
// wins a pickup / farther wins a drop at equal priority, then availability or creature appropriateness.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Task {
    bool valid, available, appropriate, Pickup; long Priority; Vec crate;
    bool IsBetterThan(const Task* other, const Vec& creature) const;
};
static float Dist2(const Vec& a, const Vec& b) { float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z; return x * x + y * y + z * z; }
bool Task::IsBetterThan(const Task* other, const Vec& creature) const
{
    float myDistance;
    float rivalDistance;
    if (!valid)
        goto fail;
    if (other) {
        if (Priority == other->Priority) {
            myDistance = Dist2(crate, creature);
            rivalDistance = Dist2(other->crate, creature);
            if (Pickup) {
                if (myDistance < rivalDistance)
                    goto avail;
                return false;
            }
            if (myDistance > rivalDistance)
                goto avail;
fail:
            return false;
        }
        if (Priority <= other->Priority)
            goto fail;
    }
avail:
    if (available)
        return true;
    return appropriate;
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    Vec me = { 0, 0, 0 };
    Task a = { true, true, false, true, 5, { 1, 0, 0 } };
    Task b = { true, true, false, true, 5, { 3, 0, 0 } };
    CHECK(a.IsBetterThan(&b, me));             // pickup: nearer wins
    CHECK(!b.IsBetterThan(&a, me));
    a.Pickup = false; b.Pickup = false;
    CHECK(!a.IsBetterThan(&b, me));            // drop: farther wins
    CHECK(b.IsBetterThan(&a, me));
    CHECK(!a.IsBetterThan(&a, me));            // equal distance never wins
    a.Priority = 7;
    CHECK(a.IsBetterThan(&b, me));             // higher priority
    CHECK(!b.IsBetterThan(&a, me));            // lower priority
    CHECK(a.IsBetterThan(0, me));              // no rival
    a.available = false;
    CHECK(!a.IsBetterThan(0, me));             // falls back to appropriateness
    a.appropriate = true;
    CHECK(a.IsBetterThan(&b, me));
    a.valid = false;
    CHECK(!a.IsBetterThan(0, me));             // creature cannot do the task
    if (g_fail) { std::printf("00861670_TEST FAIL\n"); return 1; }
    std::printf("00861670_TEST PASS\n");
    return 0;
}
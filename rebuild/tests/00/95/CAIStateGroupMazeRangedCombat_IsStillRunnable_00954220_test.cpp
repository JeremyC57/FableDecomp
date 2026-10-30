// behaviour test for 0x00954220 CAIStateGroup_MazeRangedCombat::IsStillRunnable (alive target between 8 and 50 units)
#include <cstdio>
#include <cmath>
class CThing { public: float x, y; unsigned char Dead : 1; bool IsDead() const { return Dead; } };
static bool Runnable(const CThing* self, const CThing* target, bool stopping)
{
    if (target && !target->IsDead() && !stopping) {
        float dx = target->x - self->x, dy = target->y - self->y;
        float d = (float)std::sqrt(dx * dx + dy * dy);
        if (d < 50.0f && d > 8.0f) return true;
    }
    return false;
}
int main()
{
    CThing me = { 0, 0, 0 }, mid = { 18, 24, 0 }, near_ = { 3, 4, 0 }, far_ = { 60, 0, 0 }, dead = { 30, 0, 1 };
    bool ok = Runnable(&me, &mid, false) && !Runnable(&me, &mid, true) && !Runnable(&me, &near_, false)
        && !Runnable(&me, &far_, false) && !Runnable(&me, &dead, false) && !Runnable(&me, 0, false);
    std::printf(ok ? "00954220_TEST PASS\n" : "00954220_TEST FAIL\n");
    return ok ? 0 : 1;
}
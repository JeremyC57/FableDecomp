// behaviour test for CAIStateGroup_StayNearSchool::IsRunnable (0x009167F0): logic copy over stubs
#include <cstdio>
struct V { float x, y, z; };
struct Material { long count; unsigned long flags; };
struct Building { V pos; Material* material; };
struct Kid { V pos; Building* school; bool villager; bool village; };
static bool IsNear(const Building* s, const Kid* k)
{
    float dx = k->pos.x - s->pos.x, dy = k->pos.y - s->pos.y;
    return dx * dx + dy * dy < 400.0f;
}
static bool IsRunnable(Kid* kid, bool schoolHours)
{
    Building* school = kid->school;
    if (school && kid->villager) {
        if (kid->village && schoolHours) {
            Material* m = school->material;
            if (m) {
                m->count++;
                bool result = false;
                if (m->flags & 4) result = !IsNear(school, kid);
                m->count--;
                return result;
            }
        }
    }
    return false;
}
int main()
{
    Material mat = { 1, 4 };
    Building school = { { 0, 0, 0 }, &mat };
    Kid kid = { { 30, 0, 0 }, &school, true, true };
    bool ok = IsRunnable(&kid, true) && mat.count == 1;        // 30 units away: go back near school
    kid.pos.x = 10;
    if (IsRunnable(&kid, true)) ok = false;                    // already near
    kid.pos.x = 30;
    if (IsRunnable(&kid, false)) ok = false;                   // out of school hours
    mat.flags = 0;
    if (IsRunnable(&kid, true)) ok = false;                    // not a school material
    kid.school = 0;
    if (IsRunnable(&kid, true)) ok = false;
    if (!ok) { std::printf("009167f0_TEST FAIL\n"); return 1; }
    std::printf("009167f0_TEST PASS\n");
    return 0;
}

// Behaviour test for 0x0065A8EE CTCVillage::IsTimeToGoToSchool: alive building, distance cap, school start vs travel time
#include <cstdio>
#include <cmath>
#include <cstring>

struct Vec { float X, Y, Z; };
struct Building { Vec pos; bool dead; };
struct Village { bool hasTimes; float start; };
struct Child { Vec pos; Building* work; bool member; Village* village; };

static float g_timeStep = 0.5f;
static long g_constantFPS = 30;
static float speedFor(const char* anim) { return std::strcmp(anim, "ST_RUN") == 0 ? 0.2f : 0.1f; }

static bool IsTimeToGoToSchool(const Child* child, bool run)
{
    if (!(child->work != 0 && !child->work->dead))
        return false;
    float dx = child->pos.X - child->work->pos.X, dy = child->pos.Y - child->work->pos.Y, dz = child->pos.Z - child->work->pos.Z;
    float distance = (float)std::sqrt(dx * dx + dy * dy + dz * dz);
    if (!child->member)
        return false;
    Village* village = child->village;
    if (!village)
        return false;
    if (!village->hasTimes)
        return false;
    float start = village->start / g_timeStep;
    float speed = speedFor(run ? "ST_RUN" : "ST_WALK");
    float frameSpeed = g_constantFPS * speed;
    float travel = distance * 5.0f;
    if (travel > 70.0f)
        travel = 70.0f;
    if (start > travel / frameSpeed)
        return false;
    return true;
}

int main()
{
    Building b = { { 3.0f, 4.0f, 0.0f }, false };
    Village v = { true, 1.0f };
    Child c = { { 0.0f, 0.0f, 0.0f }, &b, true, &v };
    int ok = 1;
    // distance 5 -> travel 25; walk frameSpeed 3 -> 8.33; start 1/0.5 = 2 <= 8.33 -> true
    ok = ok && IsTimeToGoToSchool(&c, false);
    // start 6 -> 12 > 25/6 (run, frameSpeed 6) = 4.17 -> false; walk: 12 > 8.33 -> false
    v.start = 6.0f; ok = ok && !IsTimeToGoToSchool(&c, true) && !IsTimeToGoToSchool(&c, false);
    // far building: travel capped at 70 -> walk 70/3 = 23.3 >= 12 -> true
    b.pos.X = 100.0f; ok = ok && IsTimeToGoToSchool(&c, false);
    b.dead = true; ok = ok && !IsTimeToGoToSchool(&c, false);
    b.dead = false; v.hasTimes = false; ok = ok && !IsTimeToGoToSchool(&c, false);
    v.hasTimes = true; c.village = 0; ok = ok && !IsTimeToGoToSchool(&c, false);
    c.village = &v; c.member = false; ok = ok && !IsTimeToGoToSchool(&c, false);
    c.member = true; c.work = 0; ok = ok && !IsTimeToGoToSchool(&c, false);
    if (!ok) { std::printf("0065a8ee_TEST FAIL\n"); return 1; }
    std::printf("0065a8ee_TEST PASS\n");
    return 0;
}
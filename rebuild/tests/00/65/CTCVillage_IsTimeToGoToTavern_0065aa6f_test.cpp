// Behaviour test for CTCVillage::IsTimeToGoToTavern (retail 0x0065AA6F): the decision logic through a copy of the model.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
static Vec Add(const Vec& a, const Vec& b) { Vec r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
static float Dist(const Vec& a, const Vec& b)
{
    float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z;
    return (float)std::sqrt(x * x + y * y + z * z);
}

struct Building { Vec Pos; bool Dead; };
struct Village;
struct Creature {
    Vec       Pos;
    Building* WorkBuilding;
    Village*  MemberOf;          // 0 = no village member component
    float     RunSpeed, WalkSpeed;
    mutable Vec LastDirection;
    mutable int SpeedCalls;
    float GetMaxRunningSpeed(const Vec& d) const { LastDirection = d; ++SpeedCalls; return RunSpeed; }
    float GetMaxWalkingSpeed(const Vec& d) const { LastDirection = d; ++SpeedCalls; return WalkSpeed; }
};
struct Village {
    bool  TavernTime;
    bool  HasTimes;
    float Start, End;
    Vec   ParentPos, ParentFacing;
    bool IsTavernTime() const { return TavernTime; }
    bool GetTavernTimes(float& start, float& end) const
    {
        if (!HasTimes) return false;
        start = Start; end = End;
        return true;
    }
    bool IsTimeToGoToTavern(Creature* creature, float daySpeed, long constantFPS, bool run, float speed)
    {
        if (IsTavernTime())
            return true;
        if (!(creature->WorkBuilding != 0 && !creature->WorkBuilding->Dead))
            return false;
        Vec position = creature->Pos;
        Vec work = creature->WorkBuilding->Pos;
        float distance = Dist(position, work);
        if (!creature->MemberOf)
            return false;
        Village* village = creature->MemberOf;
        float startFrames;
        {
            float start;
            float end;
            if (!village->GetTavernTimes(start, end))
                return false;
            startFrames = start / daySpeed;
        }
        Vec target = Add(ParentPos, ParentFacing);
        if (speed == -1.0f) {
            if (run)
                speed = creature->GetMaxRunningSpeed(target);
            else
                speed = creature->GetMaxWalkingSpeed(target);
        }
        if (startFrames > distance * 1.6f / (constantFPS * speed))
            return false;
        return true;
    }
};

int main()
{
    int bad = 0;
    Village v = { false, true, 100.0f, 200.0f, { 1.0f, 2.0f, 3.0f }, { 0.0f, 1.0f, 0.0f } };
    Building b = { { 30.0f, 40.0f, 0.0f }, false };
    Creature c = { { 0.0f, 0.0f, 0.0f }, &b, &v, 2.0f, 0.5f, { 0, 0, 0 }, 0 };
    // distance 50; 50 * 1.6 = 80.
    // already tavern time: true whatever else holds (even a dead building)
    v.TavernTime = true; b.Dead = true;
    if (!v.IsTimeToGoToTavern(&c, 1.0f, 30, false, -1.0f)) bad |= 1;
    v.TavernTime = false;
    // dead / missing work building: false
    if (v.IsTimeToGoToTavern(&c, 1.0f, 30, false, -1.0f)) bad |= 2;
    b.Dead = false; c.WorkBuilding = 0;
    if (v.IsTimeToGoToTavern(&c, 1.0f, 30, false, -1.0f)) bad |= 4;
    c.WorkBuilding = &b;
    // not a village member / no tavern times: false
    c.MemberOf = 0;
    if (v.IsTimeToGoToTavern(&c, 1.0f, 30, false, -1.0f)) bad |= 8;
    c.MemberOf = &v; v.HasTimes = false;
    if (v.IsTimeToGoToTavern(&c, 1.0f, 30, false, -1.0f)) bad |= 16;
    v.HasTimes = true;
    // explicit speed: no creature speed query. start 100 / daySpeed 50 = 2 frames; travel 80 / (30 * 1) = 2.67 -> go (true)
    c.SpeedCalls = 0;
    if (!v.IsTimeToGoToTavern(&c, 50.0f, 30, false, 1.0f)) bad |= 32;
    if (c.SpeedCalls != 0) bad |= 64;
    // start 100 / 20 = 5 frames > 2.67 -> not yet (false)
    if (v.IsTimeToGoToTavern(&c, 20.0f, 30, false, 1.0f)) bad |= 128;
    // speed -1: walking speed 0.5 towards parent pos + facing; travel 80 / (30 * 0.5) = 5.33 >= 5 -> true
    if (!v.IsTimeToGoToTavern(&c, 20.0f, 30, false, -1.0f)) bad |= 256;
    if (c.SpeedCalls != 1 || c.LastDirection.X != 1.0f || c.LastDirection.Y != 3.0f || c.LastDirection.Z != 3.0f) bad |= 512;
    // speed -1, running speed 2: travel 80 / 60 = 1.33 < 5 -> false
    if (v.IsTimeToGoToTavern(&c, 20.0f, 30, true, -1.0f)) bad |= 1024;
    if (c.SpeedCalls != 2) bad |= 2048;
    // boundary: startFrames == travel time -> true (only strictly greater is "not yet")
    b.Pos.X = 0.0f; b.Pos.Y = 0.0f; b.Pos.Z = 25.0f;   // distance 25, * 1.6 = 40; / (10 * 1) = 4
    v.Start = 8.0f;
    if (!v.IsTimeToGoToTavern(&c, 2.0f, 10, false, 1.0f)) bad |= 4096;
    if (bad) { std::printf("0065aa6f_TEST FAIL\n"); return 1; }
    std::printf("0065aa6f_TEST PASS\n");
    return 0;
}
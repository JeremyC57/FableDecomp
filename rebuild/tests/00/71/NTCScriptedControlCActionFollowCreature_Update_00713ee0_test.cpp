// Behaviour test for 00713ee0 CActionFollowCreature::Update: the follow-speed choice from the gap to the target.
#include <cstdio>
static float Max(float a, float b) { return a < b ? b : a; }
static float Min(float a, float b) { return b < a ? b : a; }
static float PickSpeed(float gap, float followRange, float targetSpeed, float maxSpeed, bool* running)
{
    float half = (followRange + 8.0f) * 0.5f;
    float speed;
    if (gap > 8.0f || (*running && gap > half)) {
        speed = Max(targetSpeed * 1.5f, maxSpeed);
        *running = true;
    } else {
        *running = false;
        if (gap <= followRange)
            speed = 0.0f;
        else if (gap < half)
            speed = targetSpeed * 0.8f;
        else
            speed = targetSpeed;
    }
    return Min(speed, maxSpeed);
}
int main()
{
    int bad = 0;
    bool running = false;
    if (PickSpeed(1.0f, 2.0f, 4.0f, 10.0f, &running) != 0.0f || running) ++bad;
    if (PickSpeed(3.0f, 2.0f, 4.0f, 10.0f, &running) != 4.0f * 0.8f) ++bad;
    if (PickSpeed(6.0f, 2.0f, 4.0f, 10.0f, &running) != 4.0f) ++bad;
    if (PickSpeed(9.0f, 2.0f, 4.0f, 10.0f, &running) != 10.0f || !running) ++bad;
    if (PickSpeed(6.0f, 2.0f, 4.0f, 10.0f, &running) != 10.0f || !running) ++bad;
    if (bad) {
        std::printf("00713ee0_TEST FAIL\n");
        return 1;
    }
    std::printf("00713ee0_TEST PASS\n");
    return 0;
}
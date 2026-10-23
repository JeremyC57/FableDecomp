// Behaviour test for 009432c0 CAIStateGroup_WorkAtSchool::Update: school-hours window (wrapping past midnight) and the seat talk length.
#include <cstdio>
static bool Over(float start, float end, float now)
{
    bool over;
    if (end > start)
        over = now > end || now < start;
    else
        over = now > end && now < start;
    return over;
}
static float SeatTalk(unsigned long r) { return (float)(r % 3 + 2); }
int main()
{
    int bad = 0;
    if (Over(0.3f, 0.6f, 0.4f)) ++bad;
    if (!Over(0.3f, 0.6f, 0.7f)) ++bad;
    if (!Over(0.3f, 0.6f, 0.2f)) ++bad;
    if (Over(0.9f, 0.1f, 0.95f)) ++bad;
    if (Over(0.9f, 0.1f, 0.05f)) ++bad;
    if (!Over(0.9f, 0.1f, 0.5f)) ++bad;
    if (SeatTalk(0) != 2.0f || SeatTalk(5) != 4.0f) ++bad;
    if (bad) {
        std::printf("009432c0_TEST FAIL\n");
        return 1;
    }
    std::printf("009432c0_TEST PASS\n");
    return 0;
}

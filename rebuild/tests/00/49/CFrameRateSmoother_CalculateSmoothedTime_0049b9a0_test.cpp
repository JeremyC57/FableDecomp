// Behaviour test for 0x0049B9A0 CFrameRateSmoother::CalculateSmoothedTime: the average of the NoTimes samples of the ring buffer starting at FirstTime (wrapping modulo MaxNoTimes), summed in float, stored as a double
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct CFrameRateSmoother {
    float* Times; int MaxNoTimes; int NoTimes; int FirstTime; double SmoothedTime;
    void CalculateSmoothedTime()
    {
        float total = 0.0f; int index = FirstTime;
        for (int i = 0; i < NoTimes; ++i) { total += Times[index]; index = (index + 1) % MaxNoTimes; }
        SmoothedTime = total / NoTimes;
    }
};
int main()
{
    float t[4] = { 1.0f, 2.0f, 4.0f, 8.0f };
    CFrameRateSmoother s; s.Times = t; s.MaxNoTimes = 4; s.NoTimes = 3; s.FirstTime = 2; s.SmoothedTime = 0;
    s.CalculateSmoothedTime(); CHECK(s.SmoothedTime == (double)(13.0f / 3));     // 4 + 8 + 1 (wraps)
    s.NoTimes = 4; s.FirstTime = 0; s.CalculateSmoothedTime(); CHECK(s.SmoothedTime == 3.75);
    if (fails == 0) printf("0049b9a0_TEST PASS\n"); else printf("0049b9a0_TEST FAIL\n");
    return fails;
}
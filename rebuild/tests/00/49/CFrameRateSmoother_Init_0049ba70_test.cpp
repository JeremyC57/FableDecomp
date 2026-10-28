// Behaviour test for 0x0049BA70 CFrameRateSmoother::Init: when the base Initialise succeeds the time buffer is resized to maxNoTimes, SmoothedTime set to 0.1f, the ring indices and count zeroed and the frame count stored; false (nothing changed) when it fails
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static bool g_initOk = true; static int g_resized = -1;
struct CFrameRateSmoother {
    int MaxNoTimes, NoTimes, NoFramesToChangeFrameRateOver, FirstTime, LastTime; double SmoothedTime;
    bool Initialise() { return g_initOk; }
    bool Init(int maxNoTimes, int frames)
    {
        if (Initialise()) {
            g_resized = maxNoTimes; SmoothedTime = 0.1f; MaxNoTimes = maxNoTimes; FirstTime = 0; LastTime = 0; NoTimes = 0;
            NoFramesToChangeFrameRateOver = frames; return true;
        }
        return false;
    }
};
int main()
{
    CFrameRateSmoother s; s.MaxNoTimes = s.NoTimes = s.FirstTime = s.LastTime = 9; s.NoFramesToChangeFrameRateOver = 9; s.SmoothedTime = 5;
    CHECK(s.Init(30, 12));
    CHECK(g_resized == 30 && s.MaxNoTimes == 30 && s.NoTimes == 0 && s.FirstTime == 0 && s.LastTime == 0 && s.NoFramesToChangeFrameRateOver == 12);
    CHECK(s.SmoothedTime == (double)0.1f);
    g_initOk = false; g_resized = -1; s.MaxNoTimes = 7;
    CHECK(!s.Init(50, 1) && g_resized == -1 && s.MaxNoTimes == 7);
    if (fails == 0) printf("0049ba70_TEST PASS\n"); else printf("0049ba70_TEST FAIL\n");
    return fails;
}
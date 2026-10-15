// Behaviour test for CV_ChapelOfEvilScript::GetMethodOfTorture (0x00E6DA40).
#include <cstdio>

static int g_rand = 0;
static int fake_rand() { return g_rand; }
static bool g_terminating = false;

struct CV_ChapelOfEvilScript {
    long TortureCount;
    long TortureType;
    bool IsActiveThreadTerminating() { return g_terminating; }
    void GetMethodOfTorture();
};

void CV_ChapelOfEvilScript::GetMethodOfTorture()
{
    if (TortureCount < 7) {
        if (IsActiveThreadTerminating())
            return;
        TortureType = TortureCount++;
    } else {
        if (IsActiveThreadTerminating())
            return;
        TortureType = fake_rand() % 7;
    }
}

int main()
{
    CV_ChapelOfEvilScript s; s.TortureCount = 0; s.TortureType = -1;
    for (int i = 0; i < 7; ++i) {
        s.GetMethodOfTorture();
        if (s.TortureType != i || s.TortureCount != i + 1) { std::printf("00e6da40_TEST FAIL\n"); return 1; }
    }
    g_rand = 45;
    s.GetMethodOfTorture();
    if (s.TortureType != 3 || s.TortureCount != 7) { std::printf("00e6da40_TEST FAIL\n"); return 1; }
    g_terminating = true; s.TortureType = -1; s.TortureCount = 2;
    s.GetMethodOfTorture();
    if (s.TortureType != -1 || s.TortureCount != 2) { std::printf("00e6da40_TEST FAIL\n"); return 1; }
    std::printf("00e6da40_TEST PASS\n");
    return 0;
}
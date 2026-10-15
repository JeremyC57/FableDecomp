// Behaviour test for CUserProfileManager::GetMaxRefreshRate (0x00406890).
#include <cstdio>

long g_maxDisplayRefreshRate = -1;
const long g_refreshRateOptions[4] = { 60, 75, 85, 100 };

struct CUserProfileManager {
    unsigned long GetMaxRefreshRate();
};

unsigned long CUserProfileManager::GetMaxRefreshRate()
{
    if (g_maxDisplayRefreshRate != -1) {
        long best = -1;
        for (unsigned long i = 0; i < 4; ++i) {
            if (g_refreshRateOptions[i] > g_maxDisplayRefreshRate)
                break;
            ++best;
        }
        return best == -1 ? 0 : best;
    }
    return 3;
}

int main()
{
    CUserProfileManager m;
    long rates[] = { -1, 30, 60, 74, 75, 85, 99, 100, 120 };
    unsigned long want[] = { 3, 0, 0, 0, 1, 2, 2, 3, 3 };
    for (int i = 0; i < 9; ++i) {
        g_maxDisplayRefreshRate = rates[i];
        if (m.GetMaxRefreshRate() != want[i]) {
            std::printf("00406890_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00406890_TEST PASS\n");
    return 0;
}

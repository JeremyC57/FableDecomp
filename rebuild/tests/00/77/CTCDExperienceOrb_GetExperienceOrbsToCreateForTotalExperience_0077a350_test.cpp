// Behaviour test for 0x0077A350 CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience(total, orbs) const
#include <cstdio>

// The def's thresholds (map keys, ascending); lower_bound(total) stepped back once unless it is the first key.
static int Split(const long* keys, int keyCount, long totalExperience, long* orbs)
{
    int n = 0;
    while (totalExperience > 0) {
        int it = 0;
        while (it < keyCount && keys[it] < totalExperience)      // lower_bound: first key >= total (keyCount = end)
            ++it;
        if (it != 0)
            --it;
        long orbExperience = keys[it] < 1 ? 1 : keys[it];        // max(it->first, 1L)
        orbs[n++] = orbExperience;
        totalExperience -= orbExperience;
    }
    return n;
}

int main()
{
    const long keys[3] = { 1, 10, 50 };
    long orbs[64];
    int n = Split(keys, 3, 25, orbs);                            // 10 + 10 + 1 x5
    if (n != 7 || orbs[0] != 10 || orbs[1] != 10 || orbs[2] != 1 || orbs[6] != 1) { std::printf("0077a350_TEST FAIL\n"); return 1; }
    n = Split(keys, 3, 10, orbs);                                // an exact key steps back: 1 x10
    if (n != 10 || orbs[0] != 1) { std::printf("0077a350_TEST FAIL\n"); return 1; }
    n = Split(keys, 3, 120, orbs);                               // past the last key: end() steps back to 50
    if (n < 3 || orbs[0] != 50 || orbs[1] != 50 || orbs[2] != 10) { std::printf("0077a350_TEST FAIL\n"); return 1; }
    if (Split(keys, 3, 0, orbs) != 0 || Split(keys, 3, -5, orbs) != 0) { std::printf("0077a350_TEST FAIL\n"); return 1; }
    const long zeroKey[2] = { 0, 5 };
    n = Split(zeroKey, 2, 3, orbs);                              // a key below 1 still yields 1-point orbs
    if (n != 3 || orbs[0] != 1) { std::printf("0077a350_TEST FAIL\n"); return 1; }
    std::printf("0077a350_TEST PASS\n");
    return 0;
}
#include <cstdio>
// Behaviour model of CTCTrespasser::CTCTrespasser (0x007BAE00): initial handle / counter state.
struct Tres {
    long inside; long deeds[3]; long cur; long knows; unsigned long informed, left; bool attached;
    Tres()
    {
        for (long i = 0; i < 3; ++i) deeds[i] = -1;
        inside = -1; knows = -1; cur = 0; informed = 0; left = 0; attached = false;
    }
};
int main()
{
    Tres t;
    if (t.inside != -1 || t.deeds[0] != -1 || t.deeds[2] != -1 || t.knows != -1 || t.cur || t.informed || t.left || t.attached) { std::printf("007bae00_TEST FAIL\n"); return 1; }
    std::printf("007bae00_TEST PASS\n");
    return 0;
}
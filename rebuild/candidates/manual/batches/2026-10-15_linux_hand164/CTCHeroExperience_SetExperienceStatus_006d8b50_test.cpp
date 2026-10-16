#include <cstdio>
// Behaviour model of CTCHeroExperience::SetExperienceStatus (0x006D8B50).
struct Exp { long total, avail; long stat[3]; unsigned int nStat; long train[2]; unsigned int nTrain; int updates; int changed;
    void Set(const long* s)
    {
        total = s[0]; avail = s[1];
        unsigned int index = 2;
        for (unsigned int i = 0; i < nStat; ++i, ++index) stat[i] = s[index];
        for (unsigned int i = 0; i < nTrain; ++i, ++index) train[i] = s[index];
        for (long k = 0; k < 10; ++k) ++updates;
        ++changed;
    } };
int main()
{
    long s[7] = { 100, 20, 1, 2, 3, 4, 5 };
    Exp e; e.nStat = 3; e.nTrain = 2; e.updates = 0; e.changed = 0;
    e.Set(s);
    if (e.total != 100 || e.avail != 20 || e.stat[0] != 1 || e.stat[2] != 3 || e.train[0] != 4 || e.train[1] != 5 || e.updates != 10 || e.changed != 1) { std::printf("006d8b50_TEST FAIL\n"); return 1; }
    std::printf("006d8b50_TEST PASS\n");
    return 0;
}

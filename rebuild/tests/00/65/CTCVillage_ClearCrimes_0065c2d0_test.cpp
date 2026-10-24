// Behaviour test for 0x0065C2D0 CTCVillage::ClearCrimes (model copy over a sorted array standing for set<long>)
#include <cstdio>
struct DeedLog { long known[8]; int n; bool Has(long h) const { for (int i = 0; i < n; ++i) if (known[i] == h) return true; return false; } };
struct Village {
    long crimes[8]; int count; bool IsEnemyBecauseOfCrime; long AmountOfFineAlreadyPaid; int removed;
    void RemoveCrime(long h) { int j = 0; for (int i = 0; i < count; ++i) if (crimes[i] != h) crimes[j++] = crimes[i]; count = j; ++removed; }
    void ClearCrimes(DeedLog* hero)
    {
        if (hero) {
            long snapshot[8]; int n = count;                 // the node walk survives erasing the current node
            for (int i = 0; i < n; ++i) snapshot[i] = crimes[i];
            for (int i = 0; i < n; ++i) { long handle = snapshot[i]; if (hero->Has(handle)) RemoveCrime(handle); }
        }
        AmountOfFineAlreadyPaid = 0;
        IsEnemyBecauseOfCrime = false;
    }
};
int main()
{
    Village v; v.count = 3; v.crimes[0] = 4; v.crimes[1] = 7; v.crimes[2] = 9; v.IsEnemyBecauseOfCrime = true; v.AmountOfFineAlreadyPaid = 50; v.removed = 0;
    DeedLog log; log.n = 2; log.known[0] = 4; log.known[1] = 9;
    v.ClearCrimes(&log);
    if (v.count != 1 || v.crimes[0] != 7 || v.removed != 2 || v.IsEnemyBecauseOfCrime || v.AmountOfFineAlreadyPaid != 0) { std::printf("0065c2d0_TEST FAIL\n"); return 1; }
    v.IsEnemyBecauseOfCrime = true; v.AmountOfFineAlreadyPaid = 5;
    v.ClearCrimes(0);                                       // no hero: only the reset
    if (v.count != 1 || v.IsEnemyBecauseOfCrime || v.AmountOfFineAlreadyPaid != 0) { std::printf("0065c2d0_TEST FAIL\n"); return 1; }
    std::printf("0065c2d0_TEST PASS\n");
    return 0;
}
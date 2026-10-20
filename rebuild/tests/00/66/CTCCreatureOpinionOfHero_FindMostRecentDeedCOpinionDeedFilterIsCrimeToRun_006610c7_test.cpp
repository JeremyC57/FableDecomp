// Behaviour test for FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard> (0x006610C7): the latest
// passing deed over pending (when asked) and every known list.
#include <cstdio>

struct Deed { long handle; unsigned long time; bool crime; };
static const Deed* Find(const Deed* pending, int np, const Deed* known, int nk, long* out, bool includePending)
{
    const Deed* best = 0; unsigned long bestTime = 0;
    if (includePending)
        for (int i = 0; i < np; ++i) {
            const Deed* d = &pending[i];
            if (d->crime && (d->time > bestTime || !best)) { if (out) *out = d->handle; bestTime = d->time; best = d; }
        }
    for (int i = 0; i < nk; ++i) {
        const Deed* d = &known[i];
        if (d->crime && (d->time > bestTime || !best)) { if (out) *out = d->handle; bestTime = d->time; best = d; }
    }
    return best;
}

int main()
{
    Deed pending[2] = { { 1, 50, true }, { 2, 70, false } };
    Deed known[3] = { { 3, 40, true }, { 4, 60, true }, { 5, 90, false } };
    long h = 0;
    if (Find(pending, 2, known, 3, &h, true) != &known[1] || h != 4) { std::printf("006610c7_TEST FAIL\n"); return 1; }
    Deed early[1] = { { 9, 0, true } };
    if (Find(early, 1, known, 0, &h, true) != &early[0] || h != 9) { std::printf("006610c7_TEST FAIL\n"); return 1; }
    if (Find(pending, 2, known, 0, 0, false) != 0) { std::printf("006610c7_TEST FAIL\n"); return 1; }
    std::printf("006610c7_TEST PASS\n");
    return 0;
}
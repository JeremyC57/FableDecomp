// Behaviour test for FindMostRecentDeed<CFilter_Bind2<IsStillHappening, TargetIs>> (0x0092E630).
#include <cstdio>

struct Deed { long type; long handle; void* target; bool happening; };
static const Deed* Find(const Deed* pending, int np, const Deed* known, int nk, long type, void* target, long* out, bool includePending)
{
    if (includePending)
        for (int i = 0; i < np; ++i) {
            const Deed* d = &pending[i];
            if (d->type == type && d->happening && d->target == target) { if (out) *out = d->handle; return d; }
        }
    for (int i = 0; i < nk; ++i) {
        const Deed* d = &known[i];
        if (d->happening && d->target == target) { if (out) *out = d->handle; return d; }
    }
    return 0;
}

int main()
{
    int a, b;
    Deed pending[2] = { { 3, 10, &a, true }, { 4, 11, &b, true } };
    Deed known[2] = { { 4, 20, &b, false }, { 4, 21, &b, true } };
    long h = 0;
    if (Find(pending, 2, known, 2, 4, &b, &h, true) != &pending[1] || h != 11) { std::printf("0092e630_TEST FAIL\n"); return 1; }
    if (Find(pending, 2, known, 2, 4, &b, &h, false) != &known[1] || h != 21) { std::printf("0092e630_TEST FAIL\n"); return 1; }
    if (Find(pending, 2, known, 2, 4, &a, 0, true) != 0) { std::printf("0092e630_TEST FAIL\n"); return 1; }
    std::printf("0092e630_TEST PASS\n");
    return 0;
}
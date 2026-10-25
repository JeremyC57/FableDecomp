// Behaviour test for 0x00C95140 libvorbis _book_maptype1_quantvals (copy of the model + logic).
#include <cstdio>
#include <cmath>
struct static_codebook { long dim; long entries; };
static long quantvals(const static_codebook* b)
{
    long vals = (long)std::floor(std::pow((double)(float)b->entries, (double)(1.f / b->dim)));
    while (1) {
        long acc = 1, acc1 = 1;
        int i;
        for (i = 0; i < b->dim; i++) { acc *= vals; acc1 *= vals + 1; }
        if (acc <= b->entries && acc1 > b->entries) return vals;
        else { if (acc > b->entries) vals--; else vals++; }
    }
}
int main()
{
    static_codebook a = { 2, 81 }, b = { 3, 100 }, c = { 1, 17 }, d = { 4, 625 }, e = { 4, 624 }, f = { 8, 6561 };
    int ok = quantvals(&a) == 9 && quantvals(&b) == 4 && quantvals(&c) == 17 && quantvals(&d) == 5 &&
             quantvals(&e) == 4 && quantvals(&f) == 3;
    if (!ok) { std::printf("00c95140_TEST FAIL\n"); return 1; }
    std::printf("00c95140_TEST PASS\n");
    return 0;
}
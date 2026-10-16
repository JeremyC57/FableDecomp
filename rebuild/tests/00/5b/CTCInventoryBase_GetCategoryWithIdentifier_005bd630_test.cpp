#include <cstdio>
// Behaviour model of CTCInventoryBase::GetCategoryWithIdentifier (0x005BD630): the first category whose def carries
// the identifier, or null; each def is held through a counted copy that is released again.
struct Def { long RefCount; long Identifier; };
struct Cat { Def* PDef; };
static Cat* Find(Cat* cats, long n, long id)
{
    for (long i = 0; i < n; ++i) {
        Def* d = cats[i].PDef; ++d->RefCount;
        bool found = d->Identifier == id;
        --d->RefCount;
        if (found) return &cats[i];
    }
    return 0;
}
int main()
{
    Def d[3] = { { 1, 10 }, { 1, 20 }, { 1, 20 } };
    Cat c[3] = { { &d[0] }, { &d[1] }, { &d[2] } };
    if (Find(c, 3, 20) != &c[1] || Find(c, 3, 30) != 0 || d[0].RefCount != 1 || d[1].RefCount != 1) { std::printf("005bd630_TEST FAIL\n"); return 1; }
    std::printf("005bd630_TEST PASS\n");
    return 0;
}
#include <cstdio>
struct Def { long RefCount; long HouseSize; int destroyed; };
struct DefPtr {
    Def* Object;
    DefPtr(Def* d) : Object(d) { if (d) ++d->RefCount; }
    ~DefPtr() { if (Object) { if (--Object->RefCount == 0) Object->destroyed++; Object = 0; } }
};
struct House { Def* def; DefPtr PeekPSimDefinition() const { return DefPtr(def); } };
static bool IsHouseLargerThan(const House* a, const House* b)
{
    if (a->PeekPSimDefinition().Object && b->PeekPSimDefinition().Object)
        return a->PeekPSimDefinition().Object->HouseSize > b->PeekPSimDefinition().Object->HouseSize;
    return false;
}
int main()
{
    Def d1 = { 1, 5, 0 }, d2 = { 1, 3, 0 };
    House a = { &d1 }, b = { &d2 }, n = { 0 };
    bool ok = IsHouseLargerThan(&a, &b) && !IsHouseLargerThan(&b, &a) && !IsHouseLargerThan(&a, &a)
        && !IsHouseLargerThan(&n, &a) && !IsHouseLargerThan(&a, &n)
        && d1.RefCount == 1 && d2.RefCount == 1 && d1.destroyed == 0;
    if (!ok) { std::printf("00512410_TEST FAIL\n"); return 1; }
    std::printf("00512410_TEST PASS\n");
    return 0;
}

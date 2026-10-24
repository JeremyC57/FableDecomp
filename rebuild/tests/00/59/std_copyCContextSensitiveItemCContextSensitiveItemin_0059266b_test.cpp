// Behaviour test for 0x0059266B std::__copy<CContextSensitiveItem*> random-access copy (model copy)
#include <cstdio>
struct Item { int ThingDefIndex; int ButtonIndex; int Priority; };
struct RandomTag { RandomTag() {} };
static Item* copy_ra(Item* first, Item* last, Item* result, const RandomTag&, int*)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Item v[5];
    for (int i = 0; i < 5; ++i) { v[i].ThingDefIndex = i; v[i].ButtonIndex = 10 + i; v[i].Priority = 20 + i; }
    // erase(v + 1): shift [2,5) down onto [1,4)
    Item* end = copy_ra(v + 2, v + 5, v + 1, RandomTag(), (int*)0);
    if (end != v + 4 || v[0].ThingDefIndex != 0 || v[1].ThingDefIndex != 2 || v[1].ButtonIndex != 12 || v[3].Priority != 24) {
        std::printf("0059266b_TEST FAIL\n"); return 1;
    }
    if (copy_ra(v + 3, v + 3, v, RandomTag(), (int*)0) != v || v[0].ThingDefIndex != 0) { std::printf("0059266b_TEST FAIL\n"); return 1; }
    std::printf("0059266b_TEST PASS\n");
    return 0;
}
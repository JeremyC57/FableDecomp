// Behaviour test for 0x005924F4 std::__uninitialized_fill_n<CContextSensitiveItem*, unsigned int, ...> (model copy)
#include <cstdio>
#include <new>
struct Item { int ThingDefIndex; int ButtonIndex; int Priority; };
struct FalseType { FalseType() {} };
static Item* fill_n_uninit(Item* first, unsigned int n, const Item& x, const FalseType&)
{
    Item* cur = first;
    for (; n > 0; --n, ++cur) new (cur) Item(x);
    return cur;
}
int main()
{
    Item buf[5];
    for (int i = 0; i < 5; ++i) { buf[i].ThingDefIndex = -1; buf[i].ButtonIndex = -1; buf[i].Priority = -1; }
    Item v; v.ThingDefIndex = 7; v.ButtonIndex = 2; v.Priority = 9;
    Item* end = fill_n_uninit(buf + 1, 3, v, FalseType());
    if (end != buf + 4 || buf[0].ThingDefIndex != -1 || buf[4].Priority != -1) { std::printf("005924f4_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 4; ++i)
        if (buf[i].ThingDefIndex != 7 || buf[i].ButtonIndex != 2 || buf[i].Priority != 9) { std::printf("005924f4_TEST FAIL\n"); return 1; }
    if (fill_n_uninit(buf, 0, v, FalseType()) != buf || buf[0].ThingDefIndex != -1) { std::printf("005924f4_TEST FAIL\n"); return 1; }
    std::printf("005924f4_TEST PASS\n");
    return 0;
}
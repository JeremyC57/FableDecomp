// behaviour test for 0x00580B2F equal_range(k): over a sorted unique-key sequence the result is [first element not less than k, first element greater than k): one element wide for a present key, empty (both at the insertion point) for an absent one, both at end past the last key
#include <cstdio>
static const int gKeys[5] = { 2, 4, 6, 8, 10 };
static int LowerBound(int k) { int i = 0; while (i < 5 && gKeys[i] < k) ++i; return i; }
static int UpperBound(int k) { int i = 0; while (i < 5 && !(k < gKeys[i])) ++i; return i; }
struct Range { int first; int second; Range(int a, int b) : first(a), second(b) { } };
static Range EqualRange(int k) { return Range(LowerBound(k), UpperBound(k)); }
int main()
{
    int fails = 0;
    Range r = EqualRange(6);
    if (r.first != 2 || r.second != 3) fails |= 1;
    r = EqualRange(5);
    if (r.first != 2 || r.second != 2) fails |= 2;
    r = EqualRange(1);
    if (r.first != 0 || r.second != 0) fails |= 4;
    r = EqualRange(11);
    if (r.first != 5 || r.second != 5) fails |= 8;
    r = EqualRange(10);
    if (r.first != 4 || r.second != 5) fails |= 16;
    if (fails) { std::printf("00580b2f_TEST FAIL %d\n", fails); return 1; }
    std::printf("00580b2f_TEST PASS\n");
    return 0;
}
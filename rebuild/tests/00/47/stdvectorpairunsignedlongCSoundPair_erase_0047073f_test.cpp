// behaviour test for 0x0047073F vector<pair<unsigned long, CSoundPair>>::erase (model: the tail moves down onto first)
#include <cstdio>
struct E { long a, b, c; };
static E* Erase(E*& finish, E* first, E* last)
{
    E* r = first;
    for (long n = finish - last; n > 0; --n) *r++ = *last++;
    finish = r;
    return first;
}
int main()
{
    E v[4] = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 }, { 4, 4, 4 } };
    E* finish = v + 4;
    E* r = Erase(finish, v + 1, v + 3);
    int ok = r == v + 1 && finish == v + 2 && v[1].a == 4 && v[1].c == 4 && v[0].b == 1;
    if (!ok) { std::printf("0047073f_TEST FAIL\n"); return 1; }
    std::printf("0047073f_TEST PASS\n");
    return 0;
}
// behaviour test for 0x0040F020: CVectorMap::LowerBound over a sorted (key, value) array
#include <cstdio>
struct P { long first; long second; };
static P* LowerBound(P* start, P* finish, long key)
{
    long count = finish - start; P* first = start;
    while (count > 0) { long half = count / 2; P* middle = first + half;
        if (middle->first < key) { first = middle + 1; count = count - half - 1; } else count = half; }
    return first;
}
int main()
{
    P v[5] = { {1, 10}, {3, 30}, {5, 50}, {7, 70}, {9, 90} };
    int ok = LowerBound(v, v + 5, 5) == v + 2 && LowerBound(v, v + 5, 6) == v + 3 && LowerBound(v, v + 5, 0) == v
          && LowerBound(v, v + 5, 10) == v + 5 && LowerBound(v, v, 3) == v;
    if (!ok) { std::printf("0040f020_TEST FAIL\n"); return 1; }
    std::printf("0040f020_TEST PASS\n");
    return 0;
}
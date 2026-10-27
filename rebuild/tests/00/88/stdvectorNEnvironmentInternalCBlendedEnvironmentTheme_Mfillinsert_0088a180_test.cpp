// behaviour test for 0x0088A180: inserting n copies keeps order (both in-place arms and growth)
#include <cstdio>
#include <vector>
int main()
{
    std::vector<int> v; v.reserve(10); v.push_back(1); v.push_back(2); v.push_back(3);
    v.insert(v.begin() + 1, 1, 9);        // elems_after (2) > n (1)
    v.insert(v.begin() + 3, 3, 8);        // elems_after (1) <= n (3)
    v.insert(v.begin(), 5, 7);            // overflow
    int want[] = { 7, 7, 7, 7, 7, 1, 9, 2, 8, 8, 8, 3 };
    if (v.size() != 12) { std::printf("0088a180_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 12; ++i) if (v[i] != want[i]) { std::printf("0088a180_TEST FAIL\n"); return 1; }
    std::printf("0088a180_TEST PASS\n");
    return 0;
}
// behaviour test for 0x006FCF90: resize shrinks by erasing the tail and grows with copies of x
#include <cstdio>
#include <vector>
int main()
{
    std::vector<long> v(5, 1);
    v.resize(2, 7); bool a = v.size() == 2 && v[1] == 1;
    v.resize(4, 7); bool b = v.size() == 4 && v[1] == 1 && v[2] == 7 && v[3] == 7;
    if (!a || !b) { std::printf("006fcf90_TEST FAIL\n"); return 1; }
    std::printf("006fcf90_TEST PASS\n");
    return 0;
}
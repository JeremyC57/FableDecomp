// behaviour test for 0x00AE7830: resize shrinks to n or grows with copies of x
#include <cstdio>
#include <vector>
enum E { E0, E1, E2 };
int main()
{
    std::vector<E> v(5, E1);
    v.resize(2, E2);
    v.resize(4, E2);
    if (v.size() != 4 || v[1] != E1 || v[2] != E2 || v[3] != E2) { std::printf("00ae7830_TEST FAIL\n"); return 1; }
    std::printf("00ae7830_TEST PASS\n");
    return 0;
}
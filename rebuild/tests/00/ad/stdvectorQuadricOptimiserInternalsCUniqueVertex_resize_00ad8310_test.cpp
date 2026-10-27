// behaviour test for 0x00AD8310: resize(n) grows with default records and shrinks by dropping the tail
#include <cstdio>
#include <vector>
struct R { int v; R() : v(42) { } };
int main()
{
    std::vector<R> v; v.resize(3); bool a = v.size() == 3 && v[2].v == 42;
    v[0].v = 1; v.resize(1); bool b = v.size() == 1 && v[0].v == 1;
    if (!a || !b) { std::printf("00ad8310_TEST FAIL\n"); return 1; }
    std::printf("00ad8310_TEST PASS\n");
    return 0;
}
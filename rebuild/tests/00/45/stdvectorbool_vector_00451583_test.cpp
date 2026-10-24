// Behaviour test for 0x00451583 std::vector<bool>::vector(n, value, a): n bytes allocated and filled with value
#include <cstdio>
#include <cstdlib>
struct BoolVector {
    bool* start; bool* finish; bool* end;
    BoolVector(unsigned int n, const bool& value) : start(0), finish(0), end(0)
    {
        start = n != 0 ? static_cast<bool*>(std::malloc(n)) : 0;
        finish = start;
        end = start + n;
        bool* p = start;
        for (; n > 0; --n, ++p)
            *p = value;
        finish = p;
    }
};
int main()
{
    BoolVector a(6, false);
    if (a.finish - a.start != 6 || a.end != a.finish) { std::printf("00451583_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (a.start[i] != false) { std::printf("00451583_TEST FAIL\n"); return 1; }
    BoolVector b(3, true);
    for (int i = 0; i < 3; ++i) if (b.start[i] != true) { std::printf("00451583_TEST FAIL\n"); return 1; }
    BoolVector c(0, true);
    if (c.start != 0 || c.finish != 0) { std::printf("00451583_TEST FAIL\n"); return 1; }
    std::free(a.start); std::free(b.start);
    std::printf("00451583_TEST PASS\n");
    return 0;
}
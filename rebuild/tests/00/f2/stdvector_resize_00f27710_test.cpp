// behaviour test for 0x00F27710 std::vector<CHeroSoulsCreatureDef >::resize(n, x): a smaller size erases exactly the tail [begin + n, end), a larger size inserts n - size() copies of x at the end, an equal size inserts nothing
#include <cstdio>
struct Elem { int W[13]; };                // the 52-byte element
static int gEraseFrom = -1, gEraseTo = -1, gInsertAt = -1, gInsertN = -1, gInsertTag = -1;
struct Vec {
    Elem* first; Elem* finish; Elem* end_of_storage;
    unsigned int size() const { return (unsigned int)(finish - first); }
    Elem* erase(Elem* a, Elem* b) { gEraseFrom = (int)(a - first); gEraseTo = (int)(b - first); finish = a; return a; }
    void fill_insert(Elem* pos, unsigned int n, const Elem& x)
    {
        gInsertAt = (int)(pos - first); gInsertN = (int)n; gInsertTag = x.W[0];
        for (unsigned int i = 0; i < n; ++i) *finish++ = x;
    }
    void resize(unsigned int n, const Elem& x)  // the function under test (STLport 4.6 shape)
    {
        if (n < size())
            erase(first + n, finish);
        else
            fill_insert(finish, n - size(), x);
    }
};
int main()
{
    int fail = 0;
    static Elem buf[16];
    Vec v; v.first = buf; v.finish = buf + 6; v.end_of_storage = buf + 16;
    Elem x; for (int i = 0; i < 13; ++i) x.W[i] = 77;
    v.resize(4, x);
    if (gEraseFrom != 4 || gEraseTo != 6 || v.size() != 4 || gInsertN != -1) fail |= 1;
    v.resize(9, x);
    if (gInsertAt != 4 || gInsertN != 5 || gInsertTag != 77 || v.size() != 9 || buf[8].W[13 - 1] != 77) fail |= 2;
    gEraseFrom = -1;
    v.resize(9, x);
    if (gInsertN != 0 || gEraseFrom != -1 || v.size() != 9) fail |= 4;
    if (fail) { std::printf("00f27710_TEST FAIL %d\n", fail); return 1; }
    std::printf("00f27710_TEST PASS\n");
    return 0;
}
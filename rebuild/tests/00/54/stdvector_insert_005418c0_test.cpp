// Behaviour test for 0x005418C0 vector<CComponent*>::insert(pos, x): with spare capacity x is constructed at the end or the tail is shifted up one slot and x stored at pos; a full vector goes to the overflow path; the result is begin() + (pos - begin()).
#include <cstdio>
#include <cstring>
static int g_overflow = 0;
struct Vec {
    int** start; int** finish; int** eos;
    void overflow(int** pos, int* const& x) { ++g_overflow; (void)pos; (void)x; }
    int** insert(int** pos, int* const& x)
    {
        unsigned int n = (unsigned int)(pos - start);
        if (finish != eos) {
            if (pos == finish) { *finish = x; ++finish; }
            else {
                *finish = finish[-1]; ++finish;
                int* copy = x;
                int num = (int)((char*)(finish - 2) - (char*)pos);
                if (num > 0) std::memmove((char*)(finish - 1) - num, pos, num);
                *pos = copy;
            }
        } else overflow(pos, x);
        return start + n;
    }
};
static int fail() { std::printf("005418c0_TEST FAIL\n"); return 1; }
int main()
{
    int a = 1, b = 2, c = 3, d = 4;
    int* storage[4] = { &a, &b, 0, 0 };
    Vec v; v.start = storage; v.finish = storage + 2; v.eos = storage + 4;
    int** r = v.insert(storage + 1, &c);               // middle: {a, c, b}
    if (r != storage + 1 || v.finish != storage + 3 || storage[0] != &a || storage[1] != &c || storage[2] != &b) return fail();
    r = v.insert(v.finish, &d);                        // at the end: {a, c, b, d}
    if (r != storage + 3 || v.finish != storage + 4 || storage[3] != &d) return fail();
    r = v.insert(storage, &a);                         // full: overflow, offset 0 still returned
    if (g_overflow != 1 || r != storage || v.finish != storage + 4) return fail();
    std::printf("005418c0_TEST PASS\n");
    return 0;
}
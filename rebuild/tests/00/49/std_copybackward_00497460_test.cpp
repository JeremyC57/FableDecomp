// behaviour test for 0x00497460 std::copy_backward over CWideString: n = last - first elements assigned back to front through the element's own operator= (n calls, last element first) into the range ending at result (safe for an overlapping right shift), result - n returned; an empty range makes no call
#include <cstdio>
static int gCalls = 0, gFirstSrc = -1;
struct TElem {                                   // stand-in for CWideString: one word with a user assignment
    int V;
    TElem& operator=(const TElem& o) { if (gCalls++ == 0) gFirstSrc = o.V; V = o.V; return *this; }
};
static TElem* Run(TElem* first, TElem* last, TElem* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
static int Check()
{
    TElem v[10];
    for (int i = 0; i < 10; ++i) v[i].V = 100 + i;
    if (Run(v + 1, v + 6, v + 8) != v + 3) return 1;        // overlapping right shift by 2: [1,6) -> [3,8)
    if (gCalls != 5 || gFirstSrc != 105) return 2;
    int want[10] = { 100, 101, 102, 101, 102, 103, 104, 105, 108, 109 };
    for (int i = 0; i < 10; ++i) if (v[i].V != want[i]) return 3;
    gCalls = 0;
    if (Run(v + 4, v + 4, v + 9) != v + 9 || gCalls != 0) return 4;   // empty range
    return 0;
}
int main()
{
    int r = Check();
    if (r) { std::printf("00497460_TEST FAIL %d\n", r); return 1; }
    std::printf("00497460_TEST PASS\n");
    return 0;
}
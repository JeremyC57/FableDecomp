// Behaviour test for 0x006721C0: STLport __linear_insert (shift the run when val < *first, else unguarded insert)
#include <cstdio>

static void CopyBackward(const int* first, const int* last, int* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
}
static void LinearInsert(int* first, int* last, int val)
{
    if (val < *first) {
        CopyBackward(first, last, last + 1);
        *first = val;
    } else {
        int* next = last;
        --next;
        while (val < *next) { *last = *next; last = next; --next; }
        *last = val;
    }
}

int main()
{
    int a[8] = { 3, 5, 9, 12, 1, 7, 12, -4 };
    for (int i = 1; i < 8; ++i) LinearInsert(a, a + i, a[i]);
    int want[8] = { -4, 1, 3, 5, 7, 9, 12, 12 };
    for (int i = 0; i < 8; ++i)
        if (a[i] != want[i]) { std::printf("006721c0_TEST FAIL\n"); return 1; }
    std::printf("006721c0_TEST PASS\n");
    return 0;
}
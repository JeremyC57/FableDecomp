// Behaviour test for 0086c620 STLport __linear_insert (labelled CCompressorZlib::SetupStream): inserting
// the value at the end of a sorted prefix keeps the records ordered by their signed key, both through
// the copy_backward branch (new minimum) and the unguarded insert branch.
#include <cstdio>

struct SValue12 { long a, b, c; };
struct SElem { long key; SValue12 v; };

static void CopyBackward(SElem* first, SElem* last, SElem* result) { while (last != first) *--result = *--last; }
static void UnguardedLinearInsert(SElem* last, SElem value)
{
    SElem* next = last - 1;
    while (value.key < next->key) { *last = *next; last = next; --next; }
    *last = value;
}
static void LinearInsert(SElem* first, SElem* last, SElem value)
{
    if (value.key < first->key) { CopyBackward(first, last, last + 1); *first = value; }
    else UnguardedLinearInsert(last, value);
}

int main()
{
    SElem e[5]; long keys[5] = { 3, 7, 9, -4, 8 };
    for (int i = 0; i < 5; ++i) { e[i].key = keys[i]; e[i].v.a = i; e[i].v.b = 0; e[i].v.c = 0; }
    for (int i = 1; i < 5; ++i) LinearInsert(e, e + i, e[i]);
    bool ok = e[0].key == -4 && e[1].key == 3 && e[2].key == 7 && e[3].key == 8 && e[4].key == 9 && e[0].v.a == 3 && e[3].v.a == 4;
    if (!ok) { std::printf("0086c620_TEST FAIL\n"); return 1; }
    std::printf("0086c620_TEST PASS\n");
    return 0;
}

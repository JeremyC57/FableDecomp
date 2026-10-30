// Behaviour test for 0x00B85B90 std::__chunk_insertion_sort<CSortElement*, int, less<CSortElement> > (STLport 4.6 _algo.c):
// every chunk_size run is sorted stably by the unsigned SortValue, then the tail.
#include <cstdio>

struct CSortElement { unsigned int SortValue; int Tag; };
static bool Less(const CSortElement& a, const CSortElement& b) { return a.SortValue < b.SortValue; }

static void UnguardedLinearInsert(CSortElement* last, CSortElement val)
{
    CSortElement* next = last - 1;
    while (Less(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(CSortElement* first, CSortElement* last, CSortElement val)
{
    if (Less(val, *first)) {
        // copy_backward(first, last, last + 1)
        for (CSortElement* s = last, *d = last + 1; s != first; ) *--d = *--s;
        *first = val;
    } else
        UnguardedLinearInsert(last, val);
}
static void InsertionSort(CSortElement* first, CSortElement* last)
{
    if (first == last) return;
    for (CSortElement* i = first + 1; i != last; ++i) LinearInsert(first, i, *i);
}
static void ChunkInsertionSort(CSortElement* first, CSortElement* last, int chunk)
{
    while (last - first >= chunk) { InsertionSort(first, first + chunk); first += chunk; }
    InsertionSort(first, last);
}

int main()
{
    int fail = 0;
    unsigned int vals[11] = { 5, 3, 0xF0000000u, 3, 1, 9, 9, 2, 8, 7, 0 };
    CSortElement e[11];
    for (int i = 0; i < 11; ++i) { e[i].SortValue = vals[i]; e[i].Tag = i; }
    ChunkInsertionSort(e, e + 11, 4);
    // chunks [5 3 F0 3] [1 9 9 2] [8 7 0]
    unsigned int want[11] = { 3, 3, 5, 0xF0000000u, 1, 2, 9, 9, 0, 7, 8 };
    int wantTag[11] = { 1, 3, 0, 2, 4, 7, 5, 6, 10, 9, 8 };
    for (int i = 0; i < 11; ++i) if (e[i].SortValue != want[i] || e[i].Tag != wantTag[i]) fail = 1;
    ChunkInsertionSort(e, e + 2, 4);                   // shorter than one chunk: only the tail sort
    if (e[0].Tag != 1 || e[1].Tag != 3) fail = 1;
    if (fail) { std::printf("00b85b90_TEST FAIL\n"); return 1; }
    std::printf("00b85b90_TEST PASS\n");
    return 0;
}
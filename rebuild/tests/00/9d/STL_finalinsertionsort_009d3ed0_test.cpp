// Behaviour test for std::__final_insertion_sort over bank entry indices ordered by data offset (0x009D3ED0).
#include <cstdio>

struct Bank { unsigned int Offsets[64]; };
struct CompareDataOffset {
    const Bank* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const { return m_pBank->Offsets[a] < m_pBank->Offsets[b]; }
};

static int g_insertionSorts = 0;
static int g_unguardedInserts = 0;

static void UnguardedLinearInsert(unsigned long* last, unsigned long value, CompareDataOffset comp)
{
    ++g_unguardedInserts;
    unsigned long* next = last - 1;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}

static void InsertionSort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    ++g_insertionSorts;
    if (first == last) return;
    for (unsigned long* i = first + 1; i != last; ++i) {
        unsigned long value = *i;
        if (comp(value, *first)) {
            for (unsigned long* p = i; p != first; --p) *p = *(p - 1);
            *first = value;
        }
        else
            UnguardedLinearInsert(i, value, comp);
    }
}

static void FinalInsertionSort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    if (last - first > 16) {
        InsertionSort(first, first + 16, comp);
        for (unsigned long* i = first + 16; i != last; ++i)
            UnguardedLinearInsert(i, *i, comp);
    }
    else
        InsertionSort(first, last, comp);
}

static int Sorted(const unsigned long* v, int n, const Bank& bank)
{
    for (int i = 1; i < n; ++i)
        if (bank.Offsets[v[i]] < bank.Offsets[v[i - 1]]) return 0;
    return 1;
}

int main()
{
    Bank bank;
    for (int i = 0; i < 64; ++i) bank.Offsets[i] = (unsigned int)((i * 37 + 11) % 64) * 100u;
    CompareDataOffset comp = { &bank };

    unsigned long small[10];
    for (int i = 0; i < 10; ++i) small[i] = (unsigned long)(9 - i);
    FinalInsertionSort(small, small + 10, comp);
    if (!Sorted(small, 10, bank) || g_insertionSorts != 1 || g_unguardedInserts == 0) { std::printf("009d3ed0_TEST FAIL\n"); return 1; }

    // the introsort loop leaves each 16-element block's minimum guard in place: the first block holds the global minimum
    unsigned long big[40];
    for (int i = 0; i < 40; ++i) big[i] = (unsigned long)((i * 7) % 40);
    unsigned long minIndex = 0;
    for (int i = 1; i < 40; ++i) if (bank.Offsets[big[i]] < bank.Offsets[big[minIndex]]) minIndex = (unsigned long)i;
    unsigned long t = big[0]; big[0] = big[minIndex]; big[minIndex] = t;
    g_insertionSorts = 0; g_unguardedInserts = 0;
    FinalInsertionSort(big, big + 40, comp);
    if (!Sorted(big, 40, bank) || g_insertionSorts != 1 || g_unguardedInserts < 24) { std::printf("009d3ed0_TEST FAIL\n"); return 1; }

    g_insertionSorts = 0;
    FinalInsertionSort(big, big, comp);
    if (g_insertionSorts != 1) { std::printf("009d3ed0_TEST FAIL\n"); return 1; }
    std::printf("009d3ed0_TEST PASS\n");
    return 0;
}
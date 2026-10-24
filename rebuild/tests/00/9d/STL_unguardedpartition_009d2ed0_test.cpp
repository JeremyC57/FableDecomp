// Behaviour test for _STL::__unguarded_partition over bank entry indices ordered by data offset (0x009D2ED0).
#include <cstdio>

struct Bank { unsigned int Offsets[32]; };
struct CompareDataOffset {
    const Bank* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const { return m_pBank->Offsets[a] < m_pBank->Offsets[b]; }
};

static unsigned long* UnguardedPartition(unsigned long* first, unsigned long* last, unsigned long pivot, CompareDataOffset comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        unsigned long tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}

int main()
{
    Bank bank;
    for (int i = 0; i < 32; ++i) bank.Offsets[i] = (unsigned int)((i * 13 + 5) % 32) * 16u;
    CompareDataOffset comp = { &bank };
    unsigned long v[20];
    for (int i = 0; i < 20; ++i) v[i] = (unsigned long)((i * 3) % 20);
    unsigned long pivot = v[10];                     // a median-of-three pick is an element of the range
    unsigned long* mid = UnguardedPartition(v, v + 20, pivot, comp);
    if (mid <= v || mid >= v + 20) { std::printf("009d2ed0_TEST FAIL\n"); return 1; }
    for (unsigned long* p = v; p != mid; ++p)
        if (bank.Offsets[*p] > bank.Offsets[pivot]) { std::printf("009d2ed0_TEST FAIL\n"); return 1; }
    for (unsigned long* p = mid; p != v + 20; ++p)
        if (bank.Offsets[*p] < bank.Offsets[pivot]) { std::printf("009d2ed0_TEST FAIL\n"); return 1; }
    int seen[20] = { 0 };
    for (int i = 0; i < 20; ++i) ++seen[v[i]];
    for (int i = 0; i < 20; ++i) if (seen[i] != 1) { std::printf("009d2ed0_TEST FAIL\n"); return 1; }
    std::printf("009d2ed0_TEST PASS\n");
    return 0;
}
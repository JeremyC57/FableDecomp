// Behaviour test for __unguarded_linear_insert<.., CEntryDataOffsetLess> (0x009D2FC0): the value is
// shifted left past every entry with a larger data offset (a smaller one sits in front as the guard).
#include <cstdio>

struct CBank {
    const unsigned int* offsets; const unsigned int* packed; unsigned int packedSize;
    unsigned int GetEntryDataOffset(unsigned int index) const
    {
        if (packedSize != 0) return packed[index - 1];
        return offsets[index];
    }
};
struct CEntryDataOffsetLess {
    const CBank* m_pBank;
    bool operator()(unsigned int a, unsigned int b) const { return m_pBank->GetEntryDataOffset(a) < m_pBank->GetEntryDataOffset(b); }
};
template <class RandomIt, class T, class Compare>
static void unguarded_linear_insert(RandomIt last, T value, Compare comp)
{
    RandomIt next = last; --next;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}

int main()
{
    static const unsigned int offsets[6] = { 0, 50, 10, 30, 40, 20 };
    CBank bank = { offsets, 0, 0 };
    CEntryDataOffsetLess less = { &bank };
    unsigned int idx[5] = { 2, 3, 4, 1, 5 };   // offsets 10 30 40 50 | 20
    unguarded_linear_insert(idx + 4, idx[4], less);
    if (idx[0] != 2 || idx[1] != 5 || idx[2] != 3 || idx[3] != 4 || idx[4] != 1) { std::printf("009d2fc0_TEST FAIL\n"); return 1; }
    static const unsigned int packed[3] = { 9, 1, 5 };
    CBank pb = { offsets, packed, 3 };
    CEntryDataOffsetLess pl = { &pb };
    unsigned int j[3] = { 2, 1, 3 };           // packed offsets 1, 9 | 5
    unguarded_linear_insert(j + 2, j[2], pl);
    if (j[0] != 2 || j[1] != 3 || j[2] != 1) { std::printf("009d2fc0_TEST FAIL\n"); return 1; }
    std::printf("009d2fc0_TEST PASS\n");
    return 0;
}
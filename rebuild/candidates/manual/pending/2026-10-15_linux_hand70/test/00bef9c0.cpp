// Behaviour test for CEntryVector_00bef9c0::push_back @ 0x00BEF9C0: in-place copy with room, the reallocating insert when full.
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };
static int g_vectorCopies = 0;
struct CRetailVector_00befbc0 {
    long m_size;
    CRetailVector_00befbc0() : m_size(0) {}
    CRetailVector_00befbc0(const CRetailVector_00befbc0& other) : m_size(other.m_size) { ++g_vectorCopies; }
};
struct SKey_00befbc0 { long m_a; long m_b; };
struct SValue_00befbc0 {
    long m_c; long m_d; CRetailVector_00befbc0 m_items;
    SValue_00befbc0() : m_c(0), m_d(0) {}
    SValue_00befbc0(const SValue_00befbc0& other) : m_c(other.m_c), m_d(other.m_d), m_items(other.m_items) {}
};
struct SEntry_00befbc0 { SKey_00befbc0 first; SValue_00befbc0 second; };

static int g_grows = 0;
struct CEntryVector_00bef9c0 {
    SEntry_00befbc0* first; SEntry_00befbc0* last; SEntry_00befbc0* end;
    void InsertGrow(SEntry_00befbc0* where, const SEntry_00befbc0& value, const SScalarTag&, unsigned long count, bool)
    { ++g_grows; if (where == last && count == 1) { last->first = value.first; last->second.m_c = value.second.m_c; ++last; ++end; } }
    void push_back(const SEntry_00befbc0& value)
    {
        if (last != end) {
            new (last) SEntry_00befbc0(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

int main()
{
    static long storage[8 * sizeof(SEntry_00befbc0) / sizeof(long) + 1];
    SEntry_00befbc0* base = reinterpret_cast<SEntry_00befbc0*>(storage);
    CEntryVector_00bef9c0 v; v.first = base; v.last = base; v.end = base + 2;
    SEntry_00befbc0 e; e.first.m_a = 1; e.first.m_b = 2; e.second.m_c = 3; e.second.m_d = 4; e.second.m_items.m_size = 5;
    v.push_back(e);
    e.first.m_a = 10; v.push_back(e);
    if (v.last != base + 2 || g_vectorCopies != 2 || g_grows != 0 || base[0].first.m_b != 2 || base[0].second.m_d != 4 ||
        base[1].first.m_a != 10 || base[1].second.m_items.m_size != 5) { std::printf("00bef9c0_TEST FAIL\n"); return 1; }
    e.first.m_a = 20; v.push_back(e);                 // full: the reallocating insert runs instead
    if (g_grows != 1 || g_vectorCopies != 2 || v.last != base + 3 || base[2].first.m_a != 20) { std::printf("00bef9c0_TEST FAIL\n"); return 1; }
    std::printf("00bef9c0_TEST PASS\n");
    return 0;
}

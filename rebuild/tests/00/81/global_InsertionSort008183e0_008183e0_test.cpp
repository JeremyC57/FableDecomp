#include <cstdio>

struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12() : m_a(0), m_b(0), m_c(0) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SPair {
    long      first;
    long      second;
    SRecord12 record;
    long      m_14, m_18, m_1c;
    SPair() : first(0), second(0), m_14(0), m_18(0), m_1c(0) { }
};
struct SCompare {                               // the comparison functor: orders by the key word
    void* m_context;
};

static int g_inserts = 0;
static bool Less(const SPair& x, const SPair& y, SCompare) { return x.first < y.first; }

static void SetPayload(SPair& e, long key)
{
    e.second = key + 1; e.record.m_a = key * 2; e.record.m_b = key * 3; e.record.m_c = -key; e.m_14 = 4 * key; e.m_18 = 5 * key; e.m_1c = 6 * key;
}

static bool PayloadOk(const SPair& e)
{
    long key = e.first;
    return e.second == key + 1 && e.record.m_a == key * 2 && e.record.m_b == key * 3 && e.record.m_c == -key && e.m_14 == 4 * key && e.m_18 == 5 * key && e.m_1c == 6 * key;
}

// STLport __linear_insert(first, last, value, comp): the out-of-line callee, reproduced for the test
static void LinearInsert(SPair* first, SPair* last, SPair value, SCompare comp)
{
    ++g_inserts;
    if (Less(value, *first, comp)) {
        for (SPair* p = last; p != first; --p)
            *p = *(p - 1);
        *first = value;
    }
    else {
        SPair* next = last;
        --next;
        while (Less(value, *next, comp)) {
            *last = *next;
            last = next;
            --next;
        }
        *last = value;
    }
}

static void InsertionSort(SPair* first, SPair* last, SCompare comp)
{
    if (first == last)
        return;
    for (SPair* i = first + 1; i != last; ++i)
        LinearInsert(first, i, *i, comp);
}

int main()
{
    bool ok = true;
    static const long keys[6] = { 5, 2, 9, 1, 7, 3 };
    static SPair v[6];
    for (int i = 0; i < 6; ++i) {
        v[i].first = keys[i];
        SetPayload(v[i], keys[i]);
    }
    SCompare comp;
    comp.m_context = 0;
    InsertionSort(v, v + 6, comp);
    if (g_inserts != 5) ok = false;
    for (int i = 0; i < 6; ++i) {
        if (i > 0 && v[i - 1].first > v[i].first) ok = false;
        if (!PayloadOk(v[i])) ok = false;
    }
    if (v[0].first != 1 || v[5].first != 9) ok = false;
    InsertionSort(v, v, comp);                  // empty range: no insertion
    InsertionSort(v + 2, v + 3, comp);          // one element: no insertion
    if (g_inserts != 5) ok = false;
    if (!ok) { std::printf("008183e0_TEST FAIL\n"); return 1; }
    std::printf("008183e0_TEST PASS\n");
    return 0;
}
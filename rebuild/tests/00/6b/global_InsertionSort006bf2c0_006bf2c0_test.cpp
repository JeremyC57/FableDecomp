#include <cstdio>

static const wchar_t* g_words[16];
struct CWideString {
    const wchar_t* m_pData;
    CWideString() : m_pData(0) { }
    CWideString(const CWideString& other) : m_pData(other.m_pData) { }
};
struct SValue {
    CWideString m_a;
    CWideString m_b;
    long        m_c;
    long        m_d;
    bool        m_e;
    SValue() : m_c(0), m_d(0), m_e(false) { }
};
struct SPair {
    long   first;
    SValue second;
    SPair() : first(0) { }
};
static const wchar_t g_pool[32] = { 0 };
struct SCompare {                               // the comparison functor: orders by the key word
    void* m_context;
};

static int g_inserts = 0;
static bool Less(const SPair& x, const SPair& y, SCompare) { return x.first < y.first; }

static void SetPayload(SPair& e, long key)
{
    e.second.m_a.m_pData = g_pool + key; e.second.m_b.m_pData = g_pool + 2 * key; e.second.m_c = 7 * key; e.second.m_d = -key; e.second.m_e = (key & 1) != 0;
}

static bool PayloadOk(const SPair& e)
{
    long key = e.first;
    return e.second.m_a.m_pData == g_pool + key && e.second.m_b.m_pData == g_pool + 2 * key && e.second.m_c == 7 * key && e.second.m_d == -key && e.second.m_e == ((key & 1) != 0);
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
    if (!ok) { std::printf("006bf2c0_TEST FAIL\n"); return 1; }
    std::printf("006bf2c0_TEST PASS\n");
    return 0;
}
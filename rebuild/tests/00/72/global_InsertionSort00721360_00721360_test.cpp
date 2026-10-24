#include <cstdio>

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct SPair {
    long       first;
    long       m_4, m_8, m_c;
    CRGBColour colour0;
    CRGBColour colour1;
    SPair() : first(0), m_4(0), m_8(0), m_c(0) { }
};
struct SCompare {                               // the comparison functor: orders by the key word
    void* m_context;
};

static int g_inserts = 0;
static bool Less(const SPair& x, const SPair& y, SCompare) { return x.first < y.first; }

static void SetPayload(SPair& e, long key)
{
    e.m_4 = key; e.m_8 = 2 * key; e.m_c = 3 * key; e.colour0.Set((unsigned char)key, (unsigned char)(key + 1), (unsigned char)(key + 2), 255);
    e.colour1.Set((unsigned char)(10 * key), 0, (unsigned char)(200 - key), (unsigned char)key);
}

static bool PayloadOk(const SPair& e)
{
    long key = e.first;
    return e.m_4 == key && e.m_8 == 2 * key && e.m_c == 3 * key && e.colour0.R == key && e.colour0.G == key + 1 && e.colour0.B == key + 2 && e.colour0.A == 255 && e.colour1.R == 10 * key && e.colour1.G == 0 && e.colour1.B == 200 - key && e.colour1.A == key;
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
    if (!ok) { std::printf("00721360_TEST FAIL\n"); return 1; }
    std::printf("00721360_TEST PASS\n");
    return 0;
}
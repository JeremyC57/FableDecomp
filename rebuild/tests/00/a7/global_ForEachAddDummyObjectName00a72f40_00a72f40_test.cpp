#include <cstdio>
#include <new>

struct __false_type { };
static int g_copies = 0, g_overflows = 0;
struct CCharString {
    const char* m_pData;
    CCharString() : m_pData(0) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }
};
struct SDummyObject {
    CCharString   m_name;
    unsigned char m_object[52];
};
static double g_heap[64];
static int g_heapUsed = 0;
struct CCharStringVector {
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    // the out-of-line STLport growth path, reproduced for the test: double the capacity, copy, append the fill
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned int fillLen, bool atEnd)
    {
        ++g_overflows;
        unsigned int size = (unsigned int)(m_pLast - m_pFirst);
        unsigned int capacity = size ? 2 * size : 1;
        CCharString* fresh = (CCharString*)&g_heap[g_heapUsed];
        g_heapUsed += (int)((capacity * sizeof(CCharString)) / sizeof(double)) + 1;
        CCharString* out = fresh;
        for (CCharString* p = m_pFirst; p != position; ++p, ++out)
            new (out) CCharString(*p);
        for (unsigned int k = 0; k < fillLen; ++k, ++out)
            new (out) CCharString(x);
        if (!atEnd)
            for (CCharString* p = position; p != m_pLast; ++p, ++out)
                new (out) CCharString(*p);
        m_pFirst = fresh;
        m_pLast = out;
        m_pEnd = fresh + capacity;
    }
    void push_back(const CCharString& x)
    {
        if (m_pLast != m_pEnd) {
            new (m_pLast) CCharString(x);
            ++m_pLast;
        }
        else
            _M_insert_overflow(m_pLast, x, __false_type(), 1, true);
    }
};
struct CAddName {
    CCharStringVector* m_pNames;
    CAddName(CCharStringVector& names) : m_pNames(&names) { }
    void operator()(const SDummyObject& object) { m_pNames->push_back(object.m_name); }
};
static CAddName ForEach(const SDummyObject* first, const SDummyObject* last, CAddName f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

int main()
{
    bool ok = true;
    static const char* names[5] = { "root", "hand_l", "hand_r", "head", "tail" };
    static SDummyObject records[5];
    for (int i = 0; i < 5; ++i)
        records[i].m_name.m_pData = names[i];
    static double initial[4];
    CCharStringVector v;
    v.m_pFirst = (CCharString*)initial;
    v.m_pLast = v.m_pFirst;
    v.m_pEnd = v.m_pFirst + 2;                  // room for two: the third name takes the overflow path
    CAddName f = ForEach(records, records + 5, CAddName(v));
    if (f.m_pNames != &v) ok = false;
    if (v.m_pLast - v.m_pFirst != 5) ok = false;
    for (int i = 0; i < 5 && ok; ++i)
        if (v.m_pFirst[i].m_pData != names[i]) ok = false;
    if (g_overflows != 2) ok = false;           // capacity 2 -> 4 -> 8
    CAddName g = ForEach(records, records, CAddName(v));   // empty range: functor returned unchanged
    if (g.m_pNames != &v || v.m_pLast - v.m_pFirst != 5) ok = false;
    if (!ok) { std::printf("00a72f40_TEST FAIL\n"); return 1; }
    std::printf("00a72f40_TEST PASS\n");
    return 0;
}
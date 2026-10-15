// Behaviour test for 0x0081D620 (CRetailVector_SPair24::PushBack_SPair24_0081d620): with room the record is copied
// in place at `last` and `last` advances by one element; when full, InsertGrow gets (last, value, tag, 1, true).
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };

struct SValue20 {                     // plain POD record: copied as a whole through re-based pointers
    long m_a, m_b, m_c, m_d, m_e;
};

struct SPair24 {                      // 24-byte element: pair<long, SValue20> with a pair-style copy constructor
    long m_key;
    SValue20 m_value;
    SPair24() { }
    SPair24(const SPair24& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct CRetailVector_SPair24 {
    SPair24* first;
    SPair24* last;
    SPair24* end;
    void InsertGrow(SPair24* where, const SPair24& value, const SScalarTag& tag, unsigned long count, bool noTail);
    void PushBack_SPair24_0081d620(const SPair24& value);
};

static SPair24* g_where = 0;
static const SPair24* g_value = 0;
static unsigned long g_count = 0;
static bool g_noTail = false;
static int g_calls = 0;

void CRetailVector_SPair24::InsertGrow(SPair24* where, const SPair24& value, const SScalarTag&, unsigned long count, bool noTail)
{
    g_where = where; g_value = &value; g_count = count; g_noTail = noTail; ++g_calls;
}

void CRetailVector_SPair24::PushBack_SPair24_0081d620(const SPair24& value)
{
    if (last != end) {
        new (last) SPair24(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

int main()
{
    SPair24 buf[2];
    SPair24 v;
    v.m_key = 100;
    v.m_value.m_a = 101;
    v.m_value.m_b = 102;
    v.m_value.m_c = 103;
    v.m_value.m_d = 104;
    v.m_value.m_e = 105;
    CRetailVector_SPair24 vec;
    vec.first = buf; vec.last = buf; vec.end = buf + 1;
    vec.PushBack_SPair24_0081d620(v);
    if (vec.last != buf + 1 || g_calls != 0 || buf[0].m_key != 100 || buf[0].m_value.m_a != 101 || buf[0].m_value.m_b != 102 || buf[0].m_value.m_c != 103 || buf[0].m_value.m_d != 104 || buf[0].m_value.m_e != 105) {
        std::printf("0081d620_TEST FAIL\n");
        return 1;
    }
    vec.PushBack_SPair24_0081d620(v);
    if (g_calls != 1 || g_where != buf + 1 || g_value != &v || g_count != 1 || !g_noTail || vec.last != buf + 1) {
        std::printf("0081d620_TEST FAIL\n");
        return 1;
    }
    std::printf("0081d620_TEST PASS\n");
    return 0;
}
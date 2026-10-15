// Behaviour test for 0x00F3B420 (CRetailVector_SRecord24::PushBack_SRecord24_00f3b420): with room the record is copied
// in place at `last` and `last` advances by one element; when full, InsertGrow gets (last, value, tag, 1, true).
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };

struct SRecord24 {                     // 24-byte element: 6 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f;
    SRecord24() { }
    SRecord24(const SRecord24& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f) { }
};

struct CRetailVector_SRecord24 {
    SRecord24* first;
    SRecord24* last;
    SRecord24* end;
    void InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned long count, bool noTail);
    void PushBack_SRecord24_00f3b420(const SRecord24& value);
};

static SRecord24* g_where = 0;
static const SRecord24* g_value = 0;
static unsigned long g_count = 0;
static bool g_noTail = false;
static int g_calls = 0;

void CRetailVector_SRecord24::InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag&, unsigned long count, bool noTail)
{
    g_where = where; g_value = &value; g_count = count; g_noTail = noTail; ++g_calls;
}

void CRetailVector_SRecord24::PushBack_SRecord24_00f3b420(const SRecord24& value)
{
    if (last != end) {
        new (last) SRecord24(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

int main()
{
    SRecord24 buf[2];
    SRecord24 v;
    v.m_a = 100;
    v.m_b = 101;
    v.m_c = 102;
    v.m_d = 103;
    v.m_e = 104;
    v.m_f = 105;
    CRetailVector_SRecord24 vec;
    vec.first = buf; vec.last = buf; vec.end = buf + 1;
    vec.PushBack_SRecord24_00f3b420(v);
    if (vec.last != buf + 1 || g_calls != 0 || buf[0].m_a != 100 || buf[0].m_b != 101 || buf[0].m_c != 102 || buf[0].m_d != 103 || buf[0].m_e != 104 || buf[0].m_f != 105) {
        std::printf("00f3b420_TEST FAIL\n");
        return 1;
    }
    vec.PushBack_SRecord24_00f3b420(v);
    if (g_calls != 1 || g_where != buf + 1 || g_value != &v || g_count != 1 || !g_noTail || vec.last != buf + 1) {
        std::printf("00f3b420_TEST FAIL\n");
        return 1;
    }
    std::printf("00f3b420_TEST PASS\n");
    return 0;
}
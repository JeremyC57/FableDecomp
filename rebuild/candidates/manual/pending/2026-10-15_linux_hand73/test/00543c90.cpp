// Behaviour test for 0x00543C90 (CRetailVector_SRecord28::PushBack_SRecord28_00543c90): with room the record is copied
// in place at `last` and `last` advances by one element; when full, InsertGrow gets (last, value, tag, 1, true).
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };

struct SRecord28 {                     // 28-byte element: 7 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f, m_g;
    SRecord28() { }
    SRecord28(const SRecord28& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f), m_g(other.m_g) { }
};

struct CRetailVector_SRecord28 {
    SRecord28* first;
    SRecord28* last;
    SRecord28* end;
    void InsertGrow(SRecord28* where, const SRecord28& value, const SScalarTag& tag, unsigned long count, bool noTail);
    void PushBack_SRecord28_00543c90(const SRecord28& value);
};

static SRecord28* g_where = 0;
static const SRecord28* g_value = 0;
static unsigned long g_count = 0;
static bool g_noTail = false;
static int g_calls = 0;

void CRetailVector_SRecord28::InsertGrow(SRecord28* where, const SRecord28& value, const SScalarTag&, unsigned long count, bool noTail)
{
    g_where = where; g_value = &value; g_count = count; g_noTail = noTail; ++g_calls;
}

void CRetailVector_SRecord28::PushBack_SRecord28_00543c90(const SRecord28& value)
{
    if (last != end) {
        new (last) SRecord28(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

int main()
{
    SRecord28 buf[2];
    SRecord28 v;
    v.m_a = 100;
    v.m_b = 101;
    v.m_c = 102;
    v.m_d = 103;
    v.m_e = 104;
    v.m_f = 105;
    v.m_g = 106;
    CRetailVector_SRecord28 vec;
    vec.first = buf; vec.last = buf; vec.end = buf + 1;
    vec.PushBack_SRecord28_00543c90(v);
    if (vec.last != buf + 1 || g_calls != 0 || buf[0].m_a != 100 || buf[0].m_b != 101 || buf[0].m_c != 102 || buf[0].m_d != 103 || buf[0].m_e != 104 || buf[0].m_f != 105 || buf[0].m_g != 106) {
        std::printf("00543c90_TEST FAIL\n");
        return 1;
    }
    vec.PushBack_SRecord28_00543c90(v);
    if (g_calls != 1 || g_where != buf + 1 || g_value != &v || g_count != 1 || !g_noTail || vec.last != buf + 1) {
        std::printf("00543c90_TEST FAIL\n");
        return 1;
    }
    std::printf("00543c90_TEST PASS\n");
    return 0;
}

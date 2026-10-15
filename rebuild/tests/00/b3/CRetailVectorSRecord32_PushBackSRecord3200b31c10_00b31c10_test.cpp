// Behaviour test for 0x00B31C10 (CRetailVector_SRecord32::PushBack_SRecord32_00b31c10): with room the record is copied
// in place at `last` and `last` advances by one element; when full, InsertGrow gets (last, value, tag, 1, true).
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };

struct SRecord32 {                     // 32-byte element: 8 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f, m_g, m_h;
    SRecord32() { }
    SRecord32(const SRecord32& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f), m_g(other.m_g), m_h(other.m_h) { }
};

struct CRetailVector_SRecord32 {
    SRecord32* first;
    SRecord32* last;
    SRecord32* end;
    void InsertGrow(SRecord32* where, const SRecord32& value, const SScalarTag& tag, unsigned long count, bool noTail);
    void PushBack_SRecord32_00b31c10(const SRecord32& value);
};

static SRecord32* g_where = 0;
static const SRecord32* g_value = 0;
static unsigned long g_count = 0;
static bool g_noTail = false;
static int g_calls = 0;

void CRetailVector_SRecord32::InsertGrow(SRecord32* where, const SRecord32& value, const SScalarTag&, unsigned long count, bool noTail)
{
    g_where = where; g_value = &value; g_count = count; g_noTail = noTail; ++g_calls;
}

void CRetailVector_SRecord32::PushBack_SRecord32_00b31c10(const SRecord32& value)
{
    if (last != end) {
        new (last) SRecord32(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

int main()
{
    SRecord32 buf[2];
    SRecord32 v;
    v.m_a = 100;
    v.m_b = 101;
    v.m_c = 102;
    v.m_d = 103;
    v.m_e = 104;
    v.m_f = 105;
    v.m_g = 106;
    v.m_h = 107;
    CRetailVector_SRecord32 vec;
    vec.first = buf; vec.last = buf; vec.end = buf + 1;
    vec.PushBack_SRecord32_00b31c10(v);
    if (vec.last != buf + 1 || g_calls != 0 || buf[0].m_a != 100 || buf[0].m_b != 101 || buf[0].m_c != 102 || buf[0].m_d != 103 || buf[0].m_e != 104 || buf[0].m_f != 105 || buf[0].m_g != 106 || buf[0].m_h != 107) {
        std::printf("00b31c10_TEST FAIL\n");
        return 1;
    }
    vec.PushBack_SRecord32_00b31c10(v);
    if (g_calls != 1 || g_where != buf + 1 || g_value != &v || g_count != 1 || !g_noTail || vec.last != buf + 1) {
        std::printf("00b31c10_TEST FAIL\n");
        return 1;
    }
    std::printf("00b31c10_TEST PASS\n");
    return 0;
}
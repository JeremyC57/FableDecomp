// Behaviour test for 0x008EC1A0 CHoverModeEntryVector::push_back: an STLport-style vector of the 4-field record with a host
// CCharString (counted copy) and an overflow path that grows the storage.
#include <cstdio>
#include <new>
static int g_copies = 0;
class CCharString { public: int id; CCharString() : id(0) {} CCharString(const CCharString& o) : id(o.id) { ++g_copies; } };
struct Rec {
    CCharString m_name; float m_offset[3]; long m_10, m_14;
};
struct __false_type { __false_type() {} };
struct Vec {
    Rec* m_start; Rec* m_finish; Rec* m_end_of_storage; Rec storage[8]; int overflows;
    Vec() : m_start(storage), m_finish(storage), m_end_of_storage(storage + 2), overflows(0) {}
    void _M_insert_overflow(Rec* pos, const Rec& x, const __false_type&, unsigned long n, bool)
    { ++overflows; m_end_of_storage = storage + 8; for (unsigned long i = 0; i < n; ++i) { new (pos) Rec(x); ++m_finish; } }
    void push_back(const Rec& x)
    {
        if (m_finish != m_end_of_storage) { new (m_finish) Rec(x); ++m_finish; }
        else _M_insert_overflow(m_finish, x, __false_type(), 1, true);
    }
};
int main()
{
    Vec v; Rec r; r.m_name.id = 7; r.m_14 = 42;
    v.push_back(r); v.push_back(r); v.push_back(r);
    if (v.m_finish - v.m_start != 3 || v.overflows != 1 || v.m_start[2].m_name.id != 7 || v.m_start[1].m_14 != 42 || g_copies != 3) {
        std::printf("008ec1a0_TEST FAIL\n"); return 1;
    }
    std::printf("008ec1a0_TEST PASS\n");
    return 0;
}
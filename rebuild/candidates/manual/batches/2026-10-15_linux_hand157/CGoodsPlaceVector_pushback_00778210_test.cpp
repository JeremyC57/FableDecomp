// Behaviour test for 0x00778210 CGoodsPlaceVector::push_back: a vector of a 36-byte plain record with a
// memberwise copy and an overflow path that grows the storage.
#include <cstdio>
#include <new>
struct Rec {
    long m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20;
    Rec() {}
    Rec(const Rec& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18), m_1c(o.m_1c), m_20(o.m_20) {}
};
struct __true_type { __true_type() {} };
struct Vec {
    Rec* m_start; Rec* m_finish; Rec* m_end_of_storage; Rec storage[4]; int overflows;
    Vec() : m_start(storage), m_finish(storage), m_end_of_storage(storage + 1), overflows(0) {}
    void _M_insert_overflow(Rec* pos, const Rec& x, const __true_type&, unsigned long n, bool)
    { ++overflows; m_end_of_storage = storage + 4; for (unsigned long i = 0; i < n; ++i) { new (pos) Rec(x); ++m_finish; } }
    void push_back(const Rec& x)
    {
        if (m_finish != m_end_of_storage) { new (m_finish) Rec(x); ++m_finish; }
        else _M_insert_overflow(m_finish, x, __true_type(), 1, true);
    }
};
int main()
{
    Vec v; Rec r; r.m_00 = 1; r.m_20 = 9;
    v.push_back(r); r.m_00 = 2; v.push_back(r);
    if (v.m_finish - v.m_start != 2 || v.overflows != 1 || v.m_start[0].m_00 != 1 || v.m_start[1].m_00 != 2 || v.m_start[1].m_20 != 9) {
        std::printf("00778210_TEST FAIL\n"); return 1;
    }
    std::printf("00778210_TEST PASS\n");
    return 0;
}

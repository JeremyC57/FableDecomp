// Behaviour test for 004cd2f0 CPairVector::push_back (in-place copy, else _M_insert_overflow)
#include <cstdio>
#include <new>
struct SPair8 { long first; long second; };
struct __true_type { __true_type() {} };
static int g_overflows = 0;
struct CPairVector {
    SPair8* m_start; SPair8* m_finish; SPair8* m_end_of_storage;
    SPair8 grown[8];
    void _M_insert_overflow(SPair8* position, const SPair8& x, const __true_type&, unsigned long fill_len, bool atend)
    {
        ++g_overflows;
        long n = (long)(position - m_start);
        for (long i = 0; i < n; ++i) grown[i] = m_start[i];
        for (unsigned long k = 0; k < fill_len; ++k) grown[n + k] = x;
        m_start = grown; m_finish = grown + n + fill_len; m_end_of_storage = grown + 8;
        if (!atend) g_overflows += 100;
    }
    void push_back(const SPair8& x);
};
void CPairVector::push_back(const SPair8& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SPair8(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __true_type(), 1, true);
}
int main()
{
    SPair8 small[1];
    CPairVector v; v.m_start = v.m_finish = small; v.m_end_of_storage = small + 1;
    SPair8 a = { 1, 2 }, b = { 3, 4 }, c = { 5, 6 };
    v.push_back(a);
    if (g_overflows != 0 || v.m_finish != small + 1 || small[0].second != 2) { std::printf("004cd2f0_TEST FAIL\n"); return 1; }
    v.push_back(b);
    if (g_overflows != 1 || v.m_start != v.grown || v.m_finish - v.m_start != 2 || v.grown[1].first != 3) { std::printf("004cd2f0_TEST FAIL\n"); return 1; }
    v.push_back(c);
    if (g_overflows != 1 || v.m_finish - v.m_start != 3 || v.grown[2].second != 6 || v.grown[0].first != 1) { std::printf("004cd2f0_TEST FAIL\n"); return 1; }
    std::printf("004cd2f0_TEST PASS\n");
    return 0;
}

// Behaviour test for 004cd2b0 CPairVectorWithFlag copy constructor (vector copy + flag)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SPair8 { long first; long second; };
struct CPairVector {
    SPair8* m_start; SPair8* m_finish; SPair8* m_end_of_storage;
    CPairVector() : m_start(0), m_finish(0), m_end_of_storage(0) {}
    CPairVector(const CPairVector& other)
    {
        unsigned n = (unsigned)(other.m_finish - other.m_start);
        m_start = n ? (SPair8*)std::malloc(n * sizeof(SPair8)) : 0;
        if (n) std::memcpy(m_start, other.m_start, n * sizeof(SPair8));
        m_finish = m_end_of_storage = m_start + n;
    }
    ~CPairVector() { std::free(m_start); }
};
struct CPairVectorWithFlag {
    CPairVector m_vector;
    unsigned char m_unused0C;
    bool m_flag;
    CPairVectorWithFlag() : m_flag(false) {}
    CPairVectorWithFlag(const CPairVectorWithFlag& other);
};
CPairVectorWithFlag::CPairVectorWithFlag(const CPairVectorWithFlag& other)
    : m_vector(other.m_vector), m_flag(other.m_flag)
{
}
int main()
{
    SPair8 items[2] = { { 1, 2 }, { 3, 4 } };
    CPairVectorWithFlag a;
    a.m_vector.m_start = items; a.m_vector.m_finish = a.m_vector.m_end_of_storage = items + 2; a.m_flag = true;
    {
        CPairVectorWithFlag b(a);
        if (!b.m_flag || b.m_vector.m_finish - b.m_vector.m_start != 2 || b.m_vector.m_start == items
            || b.m_vector.m_start[1].first != 3 || b.m_vector.m_start[1].second != 4) { std::printf("004cd2b0_TEST FAIL\n"); return 1; }
    }
    a.m_flag = false;
    CPairVectorWithFlag c(a);
    a.m_vector.m_start = a.m_vector.m_finish = a.m_vector.m_end_of_storage = 0;
    if (c.m_flag || c.m_vector.m_start[0].first != 1) { std::printf("004cd2b0_TEST FAIL\n"); return 1; }
    std::printf("004cd2b0_TEST PASS\n");
    return 0;
}
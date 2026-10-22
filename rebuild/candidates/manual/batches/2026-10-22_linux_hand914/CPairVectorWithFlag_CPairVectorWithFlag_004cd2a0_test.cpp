// Behaviour test for 004cd2a0 CPairVectorWithFlag::CPairVectorWithFlag (empty vector, flag false)
#include <cstdio>
#include <new>
struct SPair8 { long first; long second; };
struct CPairVector {
    SPair8* m_start; SPair8* m_finish; SPair8* m_end_of_storage;
    CPairVector() : m_start(0), m_finish(0), m_end_of_storage(0) {}
};
struct CPairVectorWithFlag {
    CPairVector m_vector;
    unsigned char m_unused0C;
    bool m_flag;
    CPairVectorWithFlag();
};
CPairVectorWithFlag::CPairVectorWithFlag()
    : m_flag(false)
{
}
int main()
{
    SPair8 dummy;
    union { double align; unsigned char raw[sizeof(CPairVectorWithFlag)]; } u;
    CPairVectorWithFlag* pre = (CPairVectorWithFlag*)u.raw;
    pre->m_vector.m_start = &dummy; pre->m_flag = true; pre->m_unused0C = 0x5A;
    CPairVectorWithFlag* v = new (u.raw) CPairVectorWithFlag;
    if (v->m_vector.m_start != 0 || v->m_vector.m_finish != 0 || v->m_vector.m_end_of_storage != 0 || v->m_flag
        || v->m_unused0C != 0x5A) { std::printf("004cd2a0_TEST FAIL\n"); return 1; }
    std::printf("004cd2a0_TEST PASS\n");
    return 0;
}

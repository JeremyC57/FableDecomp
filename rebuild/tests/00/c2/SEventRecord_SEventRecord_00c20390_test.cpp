#include <cstdio>
struct SEventRecord { void* m_a; void* m_b; void* m_c; unsigned char m_flag; SEventRecord() : m_a(0), m_b(0), m_c(0), m_flag(0) { } };
int main()
{
    SEventRecord r; if (r.m_a || r.m_b || r.m_c || r.m_flag) { std::printf("00c20390_TEST FAIL\n"); return 1; }

    std::printf("00c20390_TEST PASS\n");
    return 0;
}

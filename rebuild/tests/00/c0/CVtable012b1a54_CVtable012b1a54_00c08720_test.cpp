#include <cstdio>

struct CVtable_012b1a54 { virtual void v00() { } long m_index; long m_frame; void* m_pSecond; void* m_pFirst;
    CVtable_012b1a54(void* first, void* second) : m_index(-1), m_frame(-1), m_pSecond(second), m_pFirst(first) { } };
int main()
{
    int f, s; CVtable_012b1a54 a(&f, &s);
    if (a.m_index != -1 || a.m_frame != -1 || a.m_pFirst != &f || a.m_pSecond != &s) { std::printf("00c08720_TEST FAIL\n"); return 1; }
    std::printf("00c08720_TEST PASS\n"); return 0;
}
// behaviour test for 00c0efe0 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct V { virtual ~V() {} virtual void Visit(void** key) { if (m_count == m_index) m_found = key; ++m_count; }
    long m_count; long m_index; void** m_found; V(long i) : m_count(0), m_index(i), m_found(0) {} };

int main()
{
    void* keys[4] = { 0, 0, 0, 0 }; V v(2); for (int i = 0; i < 4; i++) v.Visit(&keys[i]);
    if (!(v.m_found == &keys[2] && v.m_count == 4)) { std::printf("00c0efe0_TEST FAIL\n"); return 1; }
    V w(9); for (int i = 0; i < 4; i++) w.Visit(&keys[i]);
    if (!(w.m_found == 0)) { std::printf("00c0efe0_TEST FAIL\n"); return 1; }
    std::printf("00c0efe0_TEST PASS\n");
    return 0;
}
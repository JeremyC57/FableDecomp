#include <cstdio>
struct CStreamCursor_00c62320 {
    void* m_pOwner; long m_position, m_index, m_a, m_b, m_c;
    CStreamCursor_00c62320(void* o) : m_pOwner(o), m_position(0), m_index(-1), m_a(0), m_b(0), m_c(0) { }
};
int main()
{
    int owner; CStreamCursor_00c62320 c(&owner);
    if (c.m_pOwner != &owner || c.m_position != 0 || c.m_index != -1 || c.m_a || c.m_b || c.m_c) { std::printf("00c62320_TEST FAIL\n"); return 1; }
    std::printf("00c62320_TEST PASS\n"); return 0;
}

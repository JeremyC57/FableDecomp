// Behaviour test for 0x00C5EEB0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct N { unsigned int m_blockCount, m_written; void WriteBuffer(int unused) { ++m_written; } };
int main()
{
    N n; n.m_written = 4; n.WriteBuffer(0); n.WriteBuffer(9);
    if (n.m_written != 6) { std::printf("00c5eeb0_TEST FAIL\n"); return 1; }
    std::printf("00c5eeb0_TEST PASS\n");
    return 0;
}
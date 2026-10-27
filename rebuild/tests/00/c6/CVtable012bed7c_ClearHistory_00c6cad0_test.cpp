// Behaviour test for 0x00C6CAD0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct E { unsigned char m_buffer[0x110]; unsigned char* m_pAlignedBuffer;
    void ClearHistory() { std::memset(m_pAlignedBuffer - 0x40, 0, 0x40); } };
int main()
{
    E e; std::memset(e.m_buffer, 0x5A, sizeof(e.m_buffer)); e.m_pAlignedBuffer = e.m_buffer + 0x50;
    e.ClearHistory();
    for (int i = 0; i < 0x110; ++i) {
        unsigned char want = (i >= 0x10 && i < 0x50) ? 0 : 0x5A;
        if (e.m_buffer[i] != want) { std::printf("00c6cad0_TEST FAIL\n"); return 1; }
    }
    std::printf("00c6cad0_TEST PASS\n");
    return 0;
}
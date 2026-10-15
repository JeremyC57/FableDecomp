#include <cstdio>
#include <cstring>
#include <cstdlib>
struct SStringBuffer {
    long m_a, m_b; char* m_pBuffer; char* m_pCursor; long m_capacity;
    void Init()
    {
        std::memset(this, 0, sizeof(*this));
        m_pBuffer = (char*)std::malloc(0x100);
        m_pCursor = m_pBuffer;
        *m_pCursor = 0;
        m_capacity = 0x100;
    }
};
int main()
{
    int ok = 1;
    SStringBuffer b;
    b.m_a = 5; b.m_b = 6; b.m_capacity = 7;
    b.Init();
    if (b.m_a || b.m_b || b.m_pBuffer == 0 || b.m_pCursor != b.m_pBuffer || b.m_pBuffer[0] != 0 || b.m_capacity != 0x100) ok = 0;
    std::free(b.m_pBuffer);
    if (!ok) { std::printf("00c90f10_TEST FAIL\n"); return 1; }
    std::printf("00c90f10_TEST PASS\n");
    return 0;
}
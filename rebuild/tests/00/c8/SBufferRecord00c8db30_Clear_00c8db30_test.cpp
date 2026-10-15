#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SBufferRecord_00c8db30 {
    void* m_pBuffer;
    long m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    void Clear() { std::free(m_pBuffer); std::memset(this, 0, sizeof(*this)); }
};
int main()
{
    SBufferRecord_00c8db30 r;
    r.m_pBuffer = std::malloc(64);
    r.m_4 = 4; r.m_8 = 8; r.m_c = 12; r.m_10 = 16; r.m_14 = 20; r.m_18 = 24; r.m_1c = 28;
    r.Clear();
    bool ok = r.m_pBuffer == 0 && r.m_4 == 0 && r.m_8 == 0 && r.m_c == 0 && r.m_10 == 0 && r.m_14 == 0 && r.m_18 == 0 && r.m_1c == 0;
    if (!ok) { std::printf("00c8db30_TEST FAIL\n"); return 1; }
    std::printf("00c8db30_TEST PASS\n");
    return 0;
}
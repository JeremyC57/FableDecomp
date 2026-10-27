// Test for 0x00C42720 StreamReader::Read: copies and advances, refuses a read past the end.
#include <cstdio>
#include <cstring>
struct CReader { const unsigned char* m_pBuffer; unsigned int m_position, m_size;
    bool Read(void* buf, unsigned int n) { if (m_position + n > m_size) return false; std::memcpy(buf, m_pBuffer + m_position, n); m_position += n; return true; } };
int main()
{
    const unsigned char data[7] = {1, 2, 3, 4, 5, 6, 7};
    CReader r; r.m_pBuffer = data; r.m_position = 0; r.m_size = 7;
    unsigned char out[8] = {0};
    bool ok = r.Read(out, 5) && out[0] == 1 && out[4] == 5 && r.m_position == 5;
    ok = ok && !r.Read(out, 3) && r.m_position == 5;
    ok = ok && r.Read(out, 2) && out[0] == 6 && out[1] == 7 && r.m_position == 7;
    if (!ok) { std::printf("00c42720_TEST FAIL\n"); return 1; }
    std::printf("00c42720_TEST PASS\n");
    return 0;
}
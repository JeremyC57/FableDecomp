// Behaviour test for LZO 1.08 lzo1x_9x.c len_of_coded_match (0x00C07980): encoded size of an M1..M4 match, -1 when uncodable.
#include <cstdio>
typedef unsigned int lzo_uint;
static int len_of_coded_match(lzo_uint m_len, lzo_uint m_off, lzo_uint lit)
{
    int n = 4;
    if (m_len < 2) return -1;
    if (m_len == 2) return (m_off <= 0x400 && lit > 0 && lit < 4) ? 2 : -1;
    if (m_len <= 8 && m_off <= 0x800) return 2;
    if (m_len == 3 && m_off <= 0xc00 && lit >= 4) return 2;
    if (m_off <= 0x4000) { if (m_len <= 33) return 3; m_len -= 33; while (m_len > 255) { m_len -= 255; n++; } return n; }
    if (m_off <= 0xbfff) { if (m_len <= 9) return 3; m_len -= 9; while (m_len > 255) { m_len -= 255; n++; } return n; }
    return -1;
}
int main()
{
    if (len_of_coded_match(1, 1, 0) != -1 || len_of_coded_match(2, 0x100, 2) != 2 || len_of_coded_match(2, 0x100, 0) != -1
        || len_of_coded_match(5, 0x700, 0) != 2 || len_of_coded_match(3, 0xb00, 5) != 2 || len_of_coded_match(20, 0x1000, 0) != 3
        || len_of_coded_match(33 + 600, 0x1000, 0) != 6 || len_of_coded_match(9, 0x8000, 0) != 3 || len_of_coded_match(9 + 300, 0x8000, 0) != 5
        || len_of_coded_match(10, 0xc000, 0) != -1)
    { std::printf("00c07980_TEST FAIL\n"); return 1; }
    std::printf("00c07980_TEST PASS\n"); return 0;
}
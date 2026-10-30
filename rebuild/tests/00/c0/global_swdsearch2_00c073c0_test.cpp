// Behaviour test for LZO 1.08 lzo_swd.ch swd_search2 (0x00C073C0): take the 2-byte head entry as a length-2 match.
#include <cstdio>
typedef unsigned int lzo_uint;
struct swd_t { lzo_uint m_len, m_pos, bp; lzo_uint best_pos[34]; unsigned char b[16]; unsigned short head2[65536]; };
static int swd_search2(swd_t* s)
{
    lzo_uint key = s->head2[*(unsigned short*)&s->b[s->bp]];
    if (key == 0xffffu) return 0;
    if (s->best_pos[2] == 0) s->best_pos[2] = key + 1;
    if (s->m_len < 2) { s->m_len = 2; s->m_pos = key; }
    return 1;
}
static swd_t g;
int main()
{
    for (int i = 0; i < 65536; ++i) g.head2[i] = 0xffff;
    g.b[4] = 0x34; g.b[5] = 0x12; g.bp = 4; g.m_len = 1;
    if (swd_search2(&g) != 0) { std::printf("00c073c0_TEST FAIL\n"); return 1; }
    g.head2[0x1234] = 7;
    if (swd_search2(&g) != 1 || g.m_len != 2 || g.m_pos != 7 || g.best_pos[2] != 8) { std::printf("00c073c0_TEST FAIL\n"); return 1; }
    std::printf("00c073c0_TEST PASS\n"); return 0;
}
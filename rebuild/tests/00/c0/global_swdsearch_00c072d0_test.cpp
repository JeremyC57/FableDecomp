// Behaviour test for LZO 1.08 lzo_swd.ch swd_search (0x00C072D0): walk the hash chain and keep the longest match.
#include <cstdio>
#include <cstring>
typedef unsigned int lzo_uint;
struct swd_t { lzo_uint m_len, m_pos, bp, look, nice_length; lzo_uint best_pos[34]; unsigned char b[64]; unsigned short succ3[64], best3[64]; };
static void swd_search(swd_t* s, lzo_uint node, lzo_uint cnt)
{
    lzo_uint m_len = s->m_len; const unsigned char* b = s->b; const unsigned char* bp = s->b + s->bp;
    const unsigned char* bx = s->b + s->bp + s->look; unsigned char scan_end1 = bp[m_len - 1];
    for ( ; cnt-- > 0; node = s->succ3[node])
    {
        const unsigned char* p1 = bp; const unsigned char* p2 = b + node; const unsigned char* px = bx;
        if (p2[m_len - 1] == scan_end1 && p2[m_len] == p1[m_len] && p2[0] == p1[0] && p2[1] == p1[1])
        {
            p1 += 2; p2 += 2;
            do {} while (++p1 < px && *p1 == *++p2);
            lzo_uint i = p1 - bp;
            if (i < 34 && s->best_pos[i] == 0) s->best_pos[i] = node + 1;
            if (i > m_len)
            {
                s->m_len = m_len = i; s->m_pos = node;
                if (m_len == s->look || m_len >= s->nice_length || m_len > (lzo_uint)s->best3[node]) return;
                scan_end1 = bp[m_len - 1];
            }
        }
    }
}
static swd_t g;
int main()
{
    std::memcpy(g.b, "abcdxabcdeyabcdefgz.....abcdefgh", 32);
    g.bp = 24; g.look = 8; g.m_len = 2; g.nice_length = 32;
    for (int i = 0; i < 64; ++i) g.best3[i] = 100;
    g.succ3[0] = 5; g.succ3[5] = 11; g.succ3[11] = 0;
    swd_search(&g, 0, 3);
    if (g.m_len != 7 || g.m_pos != 11 || g.best_pos[4] != 1 || g.best_pos[5] != 6) { std::printf("00c072d0_TEST FAIL\n"); return 1; }
    std::printf("00c072d0_TEST PASS\n"); return 0;
}
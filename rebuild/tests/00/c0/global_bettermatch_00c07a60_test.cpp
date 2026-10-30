// Behaviour test for LZO 1.08 lzo1x_9x.c better_match (0x00C07A60): trade match length for a cheaper offset class.
#include <cstdio>
typedef unsigned int lzo_uint;
struct swd_t { lzo_uint best_off[34]; };
static void better_match(const swd_t* swd, lzo_uint* m_len, lzo_uint* m_off)
{
    if (*m_len <= 3) return;
    if (*m_off <= 0x800) return;
    if (*m_off > 0x800 && *m_len >= 4 && *m_len <= 9 && swd->best_off[*m_len-1] && swd->best_off[*m_len-1] <= 0x800)
    { *m_len = *m_len - 1; *m_off = swd->best_off[*m_len]; return; }
    if (*m_off > 0x4000 && *m_len >= 10 && *m_len <= 10 && swd->best_off[*m_len-2] && swd->best_off[*m_len-2] <= 0x800)
    { *m_len = *m_len - 2; *m_off = swd->best_off[*m_len]; return; }
    if (*m_off > 0x4000 && *m_len >= 10 && *m_len <= 34 && swd->best_off[*m_len-1] && swd->best_off[*m_len-1] <= 0x4000)
    { *m_len = *m_len - 1; *m_off = swd->best_off[*m_len]; return; }
}
int main()
{
    swd_t s; for (int i = 0; i < 34; ++i) s.best_off[i] = 0;
    s.best_off[5] = 0x300; s.best_off[8] = 0x200; s.best_off[19] = 0x3000;
    lzo_uint l = 6, o = 0x1000; better_match(&s, &l, &o);
    if (l != 5 || o != 0x300) { std::printf("00c07a60_TEST FAIL\n"); return 1; }
    l = 10; o = 0x5000; better_match(&s, &l, &o);
    if (l != 8 || o != 0x200) { std::printf("00c07a60_TEST FAIL\n"); return 1; }
    l = 20; o = 0x5000; better_match(&s, &l, &o);
    if (l != 19 || o != 0x3000) { std::printf("00c07a60_TEST FAIL\n"); return 1; }
    l = 3; o = 0x5000; better_match(&s, &l, &o);
    if (l != 3 || o != 0x5000) { std::printf("00c07a60_TEST FAIL\n"); return 1; }
    std::printf("00c07a60_TEST PASS\n"); return 0;
}
// Behaviour test for LZO 1.08 lzo1x_9x.c find_match (0x00C07610): accept, search, fetch, then report progress.
#include <cstdio>
#include <cstring>
typedef unsigned int lzo_uint;
struct swd_t { lzo_uint m_len, m_off, look; int use_best_off, b_char; lzo_uint best_pos[34]; lzo_uint accepted; int findbest_calls, getbyte_calls; };
struct ctx { lzo_uint look, m_len, m_off, bp, ip, textsize, codesize, printcount; void (*cb)(lzo_uint, lzo_uint); };
static lzo_uint g_cb_text;
static void cb(lzo_uint t, lzo_uint) { g_cb_text = t; }
static void swd_accept(swd_t* s, lzo_uint n) { s->accepted += n; }
static void swd_findbest(swd_t* s) { s->findbest_calls++; s->m_len = 5; s->m_off = 77; }
static void swd_getbyte(swd_t* s) { s->getbyte_calls++; }
static int find_match(ctx* c, swd_t* s, lzo_uint this_len, lzo_uint skip)
{
    if (skip > 0) { swd_accept(s, this_len - skip); c->textsize += this_len - skip + 1; }
    else c->textsize += this_len - skip;
    s->m_len = 1;
    if (s->use_best_off) std::memset(s->best_pos, 0, sizeof(s->best_pos));
    swd_findbest(s);
    c->m_len = s->m_len; c->m_off = s->m_off;
    swd_getbyte(s);
    if (s->b_char < 0) { c->look = 0; c->m_len = 0; } else c->look = s->look + 1;
    c->bp = c->ip - c->look;
    if (c->cb && c->textsize > c->printcount) { (*c->cb)(c->textsize, c->codesize); c->printcount += 1024; }
    return 0;
}
int main()
{
    swd_t s; std::memset(&s, 0, sizeof(s)); s.look = 3; s.use_best_off = 1; s.best_pos[4] = 9;
    ctx c; std::memset(&c, 0, sizeof(c)); c.ip = 100; c.cb = cb; c.printcount = 5;
    find_match(&c, &s, 8, 2);
    if (s.accepted != 6 || c.textsize != 7 || c.m_len != 5 || c.m_off != 77 || c.look != 4 || c.bp != 96 || s.best_pos[4] != 0
        || g_cb_text != 7 || c.printcount != 1029 || s.getbyte_calls != 1) { std::printf("00c07610_TEST FAIL\n"); return 1; }
    s.b_char = -1; find_match(&c, &s, 1, 0);
    if (c.textsize != 8 || c.look != 0 || c.m_len != 0 || c.bp != 100 || s.accepted != 6) { std::printf("00c07610_TEST FAIL\n"); return 1; }
    std::printf("00c07610_TEST PASS\n"); return 0;
}
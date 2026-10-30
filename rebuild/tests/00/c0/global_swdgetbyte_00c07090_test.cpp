// Behaviour test for LZO 1.08 lzo_swd.ch swd_getbyte (0x00C07090): pull one input byte into the ring and advance ip/bp/rp.
#include <cstdio>
typedef unsigned int lzo_uint;
struct cstate { const unsigned char* ip; const unsigned char* in_end; };
struct swd_t { lzo_uint f, look, ip, bp, rp, b_size; unsigned char* b_wrap; cstate* c; unsigned char b[32]; };
static void swd_getbyte(swd_t* s)
{
    int c = s->c->ip < s->c->in_end ? *(s->c->ip)++ : -1;
    if (c < 0) { if (s->look > 0) --s->look; }
    else { s->b[s->ip] = (unsigned char)c; if (s->ip < s->f) s->b_wrap[s->ip] = (unsigned char)c; }
    if (++s->ip == s->b_size) s->ip = 0;
    if (++s->bp == s->b_size) s->bp = 0;
    if (++s->rp == s->b_size) s->rp = 0;
}
int main()
{
    static const unsigned char in[2] = { 0x41, 0x42 };
    cstate c = { in, in + 2 };
    swd_t s; s.f = 2; s.look = 3; s.ip = 0; s.bp = 6; s.rp = 7; s.b_size = 8; s.c = &c; s.b_wrap = s.b + 8;
    swd_getbyte(&s);
    if (s.b[0] != 0x41 || s.b[8] != 0x41 || s.ip != 1 || s.bp != 7 || s.rp != 0) { std::printf("00c07090_TEST FAIL\n"); return 1; }
    swd_getbyte(&s); swd_getbyte(&s);
    if (s.b[1] != 0x42 || s.look != 2 || s.ip != 3 || s.bp != 1) { std::printf("00c07090_TEST FAIL\n"); return 1; }
    std::printf("00c07090_TEST PASS\n"); return 0;
}
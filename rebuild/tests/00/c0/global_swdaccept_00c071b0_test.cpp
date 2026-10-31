// Behaviour test for swd_accept 0x00C071B0 (LZO 1.08 lzo_swd.ch): a copy of the model on a heap window.
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef unsigned int   lzo_uint32;
typedef unsigned short swd_uint;
#define SWD_N 0xbfff
#define SWD_F 2048
#define SWD_HSIZE 16384
#define NIL2 0xffffu

struct lzo1x_999_t { const lzo_byte* ip; const lzo_byte* in_end; };
struct lzo1x_999_swd_t {
    lzo_uint n, f, look, ip, bp, rp, b_size, node_count;
    lzo_byte* b_wrap;
    lzo1x_999_t* c;
    lzo_byte b[SWD_N + SWD_F + SWD_F];
    swd_uint head3[SWD_HSIZE];
    swd_uint succ3[SWD_N + SWD_F];
    swd_uint best3[SWD_N + SWD_F];
    swd_uint llen3[SWD_HSIZE];
    swd_uint head2[65536L];
};
typedef lzo1x_999_swd_t* lzo_swd_p;

#define HEAD3(b,p) (((0x9f5f*(((((lzo_uint32)b[p]<<5)^b[p+1])<<5)^b[p+2]))>>5) & (SWD_HSIZE-1))
#define HEAD2(b,p) ((swd_uint)(b[p] | (b[p+1] << 8)))   /* little-endian unaligned 16-bit read */
#define getbyte(c) ((c).ip < (c).in_end ? *((c).ip)++ : (-1))

static void swd_getbyte(lzo_swd_p s)
{
    int c;
    if ((c = getbyte(*(s->c))) < 0) {
        if (s->look > 0)
            --s->look;
    } else {
        s->b[s->ip] = (lzo_byte)c;
        if (s->ip < s->f)
            s->b_wrap[s->ip] = (lzo_byte)c;
    }
    if (++s->ip == s->b_size) s->ip = 0;
    if (++s->bp == s->b_size) s->bp = 0;
    if (++s->rp == s->b_size) s->rp = 0;
}
static void swd_remove_node(lzo_swd_p s, lzo_uint node)
{
    if (s->node_count == 0) {
        lzo_uint key = HEAD3(s->b, node);
        --s->llen3[key];
        key = HEAD2(s->b, node);
        if ((lzo_uint)s->head2[key] == node)
            s->head2[key] = NIL2;
    } else
        --s->node_count;
}
static void swd_accept(lzo_swd_p s, lzo_uint n)
{
    while (n--) {
        lzo_uint key;
        swd_remove_node(s, s->rp);
        key = HEAD3(s->b, s->bp);
        s->succ3[s->bp] = s->head3[key];
        s->head3[key] = (swd_uint)s->bp;
        s->best3[s->bp] = (swd_uint)(s->f + 1);
        s->llen3[key]++;
        key = HEAD2(s->b, s->bp);
        s->head2[key] = (swd_uint)s->bp;
        swd_getbyte(s);
    }
}

static int fail(int line) { std::printf("00c071b0_TEST FAIL (line %d)\n", line); return 1; }

int main()
{
    lzo1x_999_swd_t* s = (lzo1x_999_swd_t*)std::calloc(1, sizeof(lzo1x_999_swd_t));
    lzo1x_999_t c;
    static const lzo_byte input[4] = { 7, 8, 9, 10 };
    c.ip = input; c.in_end = input + 4;
    s->c = &c;
    s->f = SWD_F; s->b_size = SWD_N + SWD_F; s->b_wrap = s->b + s->b_size;
    s->look = 10; s->node_count = 2;
    for (int i = 0; i < 16; ++i) s->b[i] = (lzo_byte)(i + 1);   // 1, 2, 3, ...
    s->bp = 0; s->rp = 0; s->ip = 6;
    for (int i = 0; i < SWD_HSIZE; ++i) s->head3[i] = 0x1234;

    swd_accept(s, 0);                                          // n == 0: nothing happens
    if (s->bp != 0 || s->ip != 6 || s->node_count != 2) return fail(__LINE__);

    swd_accept(s, 3);
    // HEAD3 of bytes (1,2,3): ((0x9f5f * ((((1<<5)^2)<<5)^3)) >> 5) & 0x3fff = 14734
    if (HEAD3(s->b, 0) != 14734) return fail(__LINE__);
    if (s->head3[14734] != 0 || s->succ3[0] != 0x1234 || s->llen3[14734] != 1) return fail(__LINE__);
    if (s->best3[0] != SWD_F + 1 || s->best3[2] != SWD_F + 1) return fail(__LINE__);
    if (s->head2[1 | (2 << 8)] != 0 || s->head2[2 | (3 << 8)] != 1 || s->head2[3 | (4 << 8)] != 2) return fail(__LINE__);
    if (s->bp != 3 || s->rp != 3 || s->ip != 9) return fail(__LINE__);
    if (s->node_count != 0) return fail(__LINE__);              // two of the three removals only counted down
    if (s->b[6] != 7 || s->b[7] != 8 || s->b[8] != 9) return fail(__LINE__);   // fetched input bytes
    // the third removal (node_count 0) unlinked rp == 2 (llen3 of bytes 3,4,5 down to 0xffff) before bp == 2 was
    // inserted with the same key (back up to 0)
    if (HEAD3(s->b, 2) == HEAD3(s->b, 0) || s->llen3[HEAD3(s->b, 2)] != 0) return fail(__LINE__);
    if (s->llen3[HEAD3(s->b, 1)] != 1) return fail(__LINE__);

    std::free(s);
    std::printf("00c071b0_TEST PASS\n");
    return 0;
}
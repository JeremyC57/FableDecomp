// Behaviour test for 0x00C071B0 LZO 1.08 swd_accept (parked row): n == 0 is a no-op; each accepted byte unlinks the rp
// node (or counts node_count down), links bp into head3/succ3/best3/llen3 and head2, then advances the ring positions.
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned short swd_uint;
#define SWD_N 64
#define SWD_F 8
#define SWD_HSIZE 16384
struct Ctx { const unsigned char* ip; const unsigned char* in_end; };
struct Swd {
    unsigned int n, f, look, ip, bp, rp, b_size, node_count;
    unsigned char* b_wrap;
    Ctx* c;
    unsigned char b[SWD_N + SWD_F + SWD_F];
    swd_uint head3[SWD_HSIZE], succ3[SWD_N + SWD_F], best3[SWD_N + SWD_F], llen3[SWD_HSIZE], head2[65536];
};
#define HEAD3(b,p) (((0x9f5fu*(((((unsigned int)b[p]<<5)^b[p+1])<<5)^b[p+2]))>>5) & (SWD_HSIZE-1))
static unsigned int HEAD2(const unsigned char* b, unsigned int p) { return (unsigned int)b[p] | ((unsigned int)b[p + 1] << 8); }

static void swd_getbyte(Swd* s)
{
    int c = s->c->ip < s->c->in_end ? *(s->c->ip)++ : -1;
    if (c < 0) { if (s->look > 0) --s->look; }
    else { s->b[s->ip] = (unsigned char)c; if (s->ip < s->f) s->b_wrap[s->ip] = (unsigned char)c; }
    if (++s->ip == s->b_size) s->ip = 0;
    if (++s->bp == s->b_size) s->bp = 0;
    if (++s->rp == s->b_size) s->rp = 0;
}
static void swd_remove_node(Swd* s, unsigned int node)
{
    if (s->node_count == 0) {
        unsigned int key = HEAD3(s->b, node);
        --s->llen3[key];
        key = HEAD2(s->b, node);
        if ((unsigned int)s->head2[key] == node) s->head2[key] = 0xffff;
    }
    else --s->node_count;
}
static void swd_accept(Swd* s, unsigned int n)
{
    if (n) do {
        unsigned int key;
        swd_remove_node(s, s->rp);
        key = HEAD3(s->b, s->bp);
        s->succ3[s->bp] = s->head3[key];
        s->head3[key] = (swd_uint)s->bp;
        s->best3[s->bp] = (swd_uint)(s->f + 1);
        s->llen3[key]++;
        key = HEAD2(s->b, s->bp);
        s->head2[key] = (swd_uint)s->bp;
        swd_getbyte(s);
    } while (--n != 0);
}

int main()
{
    Swd* s = (Swd*)std::calloc(1, sizeof(Swd));
    static const unsigned char input[] = "abcabcabd";
    Ctx c = { input, input + 9 };
    s->c = &c; s->n = SWD_N; s->f = SWD_F; s->b_size = SWD_N; s->b_wrap = s->b + SWD_N;
    std::memcpy(s->b, "xyzxyzxyzxyz", 12);
    s->bp = 0; s->rp = 40; s->ip = 12; s->node_count = 1; s->look = 5;
    std::memset(s->head2, 0xff, sizeof(s->head2));
    swd_accept(s, 0);
    int bad = s->bp != 0 || s->node_count != 1;
    unsigned int k0 = HEAD3(s->b, 0), k3 = HEAD3(s->b, 3);
    swd_accept(s, 4);                                            // positions 0..3 inserted
    bad |= s->bp != 4 || s->rp != 44 || s->ip != 16 || s->node_count != 0;
    bad |= k0 != k3 || s->head3[k0] != 3 || s->succ3[3] != 0 || s->llen3[k0] != 2 || s->best3[0] != SWD_F + 1;
    bad |= s->head2[HEAD2(s->b, 0)] != 3 || s->b[12] != 'a' || s->b[15] != 'a' || c.ip != input + 4;
    // a full ring wrap: rp reaches b_size and resets
    s->rp = SWD_N - 1; s->bp = 10; s->ip = 20;
    s->llen3[HEAD3(s->b, SWD_N - 1)] = 1;
    swd_accept(s, 1);
    bad |= s->rp != 0 || s->bp != 11 || s->ip != 21;
    std::free(s);
    if (bad) { std::printf("00c071b0_TEST FAIL\n"); return 1; }
    std::printf("00c071b0_TEST PASS\n");
    return 0;
}

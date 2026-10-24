#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define CDECL __cdecl
#define FASTCALL __fastcall
#else
#define CDECL
#define FASTCALL
#endif
typedef unsigned char lzo_byte; typedef unsigned int lzo_uint; typedef unsigned int lzo_uint32; typedef unsigned short swd_uint;
typedef void (CDECL *lzo_progress_callback_t)(lzo_uint, lzo_uint);
#define SWD_N 0xbfff
#define SWD_F 2048
#define SWD_HSIZE 16384
#define NIL2 0xffffu
typedef struct {
    int init; lzo_uint look; lzo_uint m_len; lzo_uint m_off; lzo_uint last_m_len; lzo_uint last_m_off;
    const lzo_byte* bp; const lzo_byte* ip; const lzo_byte* in; const lzo_byte* in_end; lzo_byte* out;
    lzo_progress_callback_t cb; lzo_uint textsize; lzo_uint codesize; lzo_uint printcount;
    unsigned int lit_bytes, match_bytes, rep_bytes, lazy; lzo_uint r1_lit; lzo_uint r1_m_len;
    unsigned int m1a_m, m1b_m, m2_m, m3_m, m4_m; unsigned int lit1_r, lit2_r, lit3_r;
} lzo1x_999_t;
typedef struct {
    lzo_uint n, f, threshold, max_chain, nice_length; int use_best_off; lzo_uint lazy_insert, m_len, m_off, look;
    int b_char; lzo_uint best_off[34]; lzo1x_999_t* c; lzo_uint m_pos; lzo_uint best_pos[34];
    const lzo_byte* dict; const lzo_byte* dict_end; lzo_uint dict_len; lzo_uint ip, bp, rp, b_size;
    lzo_byte* b_wrap; lzo_uint node_count; lzo_uint first_rp;
    lzo_byte b[SWD_N + SWD_F + SWD_F]; swd_uint head3[SWD_HSIZE]; swd_uint succ3[SWD_N + SWD_F];
    swd_uint best3[SWD_N + SWD_F]; swd_uint llen3[SWD_HSIZE]; swd_uint head2[65536L];
} lzo1x_999_swd_t;
typedef lzo1x_999_swd_t* lzo_swd_p;
#define HEAD3(b,p) (((0x9f5f*(((((lzo_uint32)b[p]<<5)^b[p+1])<<5)^b[p+2]))>>5) & (SWD_HSIZE-1))
static unsigned HEAD2f(const lzo_byte* b, lzo_uint p) { return (unsigned)(b[p] | (b[p+1] << 8)); }   // little-endian 16-bit read
#define HEAD2(b,p) HEAD2f(b,p)
static void FASTCALL swd_remove_node(lzo_swd_p s, lzo_uint node)
{
    if (s->node_count == 0) {
        lzo_uint key;
        key = HEAD3(s->b,node);
        --s->llen3[key];
        key = HEAD2(s->b,node);
        if ((lzo_uint) s->head2[key] == node)
            s->head2[key] = NIL2;
    }
    else
        --s->node_count;
}
static lzo1x_999_swd_t g_s;
int main()
{
    int bad = 0;
    g_s.b[10] = 'a'; g_s.b[11] = 'b'; g_s.b[12] = 'c';
    lzo_uint k3 = HEAD3(g_s.b, 10), k2 = 'a' | ('b' << 8);
    g_s.node_count = 2; swd_remove_node(&g_s, 10);
    if (g_s.node_count != 1 || g_s.llen3[k3] != 0) bad = 1;
    g_s.node_count = 0; g_s.llen3[k3] = 3; g_s.head2[k2] = 10;
    swd_remove_node(&g_s, 10);
    if (g_s.llen3[k3] != 2 || g_s.head2[k2] != NIL2) bad = 2;
    g_s.head2[k2] = 7; swd_remove_node(&g_s, 10);
    if (g_s.llen3[k3] != 1 || g_s.head2[k2] != 7) bad = 3;
    if (bad) { std::printf("00c07140_TEST FAIL\n"); return 1; }
    std::printf("00c07140_TEST PASS\n");
    return 0;
}
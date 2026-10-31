// Behaviour test for 0x00C07420 swd_findbest (LZO 1.08 lzo_swd.ch): a copy of the model and the upstream bodies.
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef unsigned long  lzo_uint32;
typedef unsigned short swd_uint;
typedef void (*lzo_progress_callback_t)(lzo_uint, lzo_uint);

#define SWD_N          0xbfff          /* M4_MAX_OFFSET: size of the ring buffer */
#define SWD_F          2048            /* upper limit for match length */
#define SWD_HSIZE      16384
#define SWD_MAX_CHAIN  2048
#define SWD_BEST_OFF   (34 + 1 - 1)    /* LZO_MAX3(M2_MAX_LEN, M3_MAX_LEN, M4_MAX_LEN) + 1, minus one unused */
#define NIL2           0xffffu

typedef struct
{
    int init;                  // +0x00
    lzo_uint look;             // +0x04
    lzo_uint m_len;            // +0x08
    lzo_uint m_off;            // +0x0c
    lzo_uint last_m_len;       // +0x10
    lzo_uint last_m_off;       // +0x14
    const lzo_byte* bp;        // +0x18
    const lzo_byte* ip;        // +0x1c
    const lzo_byte* in;        // +0x20
    const lzo_byte* in_end;    // +0x24
    lzo_byte* out;             // +0x28
    lzo_progress_callback_t cb;// +0x2c
    lzo_uint textsize;         // +0x30
    lzo_uint codesize;         // +0x34
    lzo_uint printcount;       // +0x38
    unsigned long lit_bytes;   // +0x3c
    unsigned long match_bytes; // +0x40
    unsigned long rep_bytes;   // +0x44
    unsigned long lazy;        // +0x48
    lzo_uint r1_lit;           // +0x4c
    lzo_uint r1_m_len;         // +0x50
    unsigned long m1a_m, m1b_m, m2_m, m3_m, m4_m;   // +0x54..+0x64
    unsigned long lit1_r, lit2_r, lit3_r;           // +0x68..+0x70
} lzo1x_999_t;                 // 0x74

typedef struct
{
    lzo_uint n;                // +0x000
    lzo_uint f;                // +0x004
    lzo_uint threshold;        // +0x008
    lzo_uint max_chain;        // +0x00c
    lzo_uint nice_length;      // +0x010
    int use_best_off;          // +0x014
    lzo_uint lazy_insert;      // +0x018
    lzo_uint m_len;            // +0x01c
    lzo_uint m_off;            // +0x020
    lzo_uint look;             // +0x024
    int b_char;                // +0x028
    lzo_uint best_off[34];     // +0x02c
    lzo1x_999_t* c;            // +0x0b4
    lzo_uint m_pos;            // +0x0b8
    lzo_uint best_pos[34];     // +0x0bc
    const lzo_byte* dict;      // +0x144
    const lzo_byte* dict_end;  // +0x148
    lzo_uint dict_len;         // +0x14c
    lzo_uint ip;               // +0x150
    lzo_uint bp;               // +0x154
    lzo_uint rp;               // +0x158
    lzo_uint b_size;           // +0x15c
    lzo_byte* b_wrap;          // +0x160
    lzo_uint node_count;       // +0x164
    lzo_uint first_rp;         // +0x168
    lzo_byte b[SWD_N + SWD_F + SWD_F];   // +0x16c
    swd_uint head3[SWD_HSIZE];           // +0xd16c
    swd_uint succ3[SWD_N + SWD_F];       // +0x1516c
    swd_uint best3[SWD_N + SWD_F];       // +0x2e16a
    swd_uint llen3[SWD_HSIZE];           // +0x47168
    swd_uint head2[65536L];              // +0x4f168
} lzo1x_999_swd_t;                       // 0x6f168
typedef lzo1x_999_swd_t* lzo_swd_p;

#define HEAD3(b,p) \
    (((0x9f5f*(((((lzo_uint32)b[p]<<5)^b[p+1])<<5)^b[p+2]))>>5) & (SWD_HSIZE-1))
#define HEAD2(b,p)      (* (unsigned short*) &(b[p]))   /* LZO_UNALIGNED_OK_2 */

#define getbyte(c)  ((c).ip < (c).in_end ? *((c).ip)++ : (-1))
#define LZO_BYTE(x) ((unsigned char) (x))
#define SWD_UINT(x) ((swd_uint)(x))
#define s_get_head3(s,key) s->head3[key]
#define THRESHOLD 1

typedef int lzo_bool;
#define SWD_THRESHOLD 1
#define LZO_E_OK 0
#define swd_pos2off(s,pos) \
    (s->bp > (pos) ? s->bp - (pos) : s->b_size - ((pos) - s->bp))
#define lzo_memset memset
#define swd_end(s)


static void swd_remove_node(lzo_swd_p s, lzo_uint node)
{
    if (s->node_count == 0)
    {
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

static void swd_search(lzo_swd_p s, lzo_uint node, lzo_uint cnt)
{
    const lzo_byte *p1;
    const lzo_byte *p2;
    const lzo_byte *px;
    lzo_uint m_len = s->m_len;
    const lzo_byte * b  = s->b;
    const lzo_byte * bp = s->b + s->bp;
    const lzo_byte * bx = s->b + s->bp + s->look;
    unsigned char scan_end1;

    scan_end1 = bp[m_len - 1];
    for ( ; cnt-- > 0; node = s->succ3[node])
    {
        p1 = bp;
        p2 = b + node;
        px = bx;

        if (
            p2[m_len - 1] == scan_end1 &&
            p2[m_len] == p1[m_len] &&
            p2[0] == p1[0] &&
            p2[1] == p1[1])
        {
            lzo_uint i;

            p1 += 2; p2 += 2;
            do {} while (++p1 < px && *p1 == *++p2);
            i = p1 - bp;

            if (i < SWD_BEST_OFF)
            {
                if (s->best_pos[i] == 0)
                    s->best_pos[i] = node + 1;
            }
            if (i > m_len)
            {
                s->m_len = m_len = i;
                s->m_pos = node;
                if (m_len == s->look)
                    return;
                if (m_len >= s->nice_length)
                    return;
                if (m_len > (lzo_uint) s->best3[node])
                    return;
                scan_end1 = bp[m_len - 1];
            }
        }
    }
}

static lzo_bool swd_search2(lzo_swd_p s)
{
    lzo_uint key;

    key = s->head2[ HEAD2(s->b,s->bp) ];
    if (key == NIL2)
        return 0;
    if (s->best_pos[2] == 0)
        s->best_pos[2] = key + 1;

    if (s->m_len < 2)
    {
        s->m_len = 2;
        s->m_pos = key;
    }
    return 1;
}

static void swd_findbest(lzo_swd_p s)
{
    lzo_uint key;
    lzo_uint cnt, node;
    lzo_uint len;

    /* get current head, add bp into HEAD3 */
    key = HEAD3(s->b,s->bp);
    node = s->succ3[s->bp] = s_get_head3(s,key);
    cnt = s->llen3[key]++;
    if (cnt > s->max_chain && s->max_chain > 0)
        cnt = s->max_chain;
    s->head3[key] = SWD_UINT(s->bp);

    s->b_char = s->b[s->bp];
    len = s->m_len;
    if (s->m_len >= s->look)
    {
        if (s->look == 0)
            s->b_char = -1;
        s->m_off = 0;
        s->best3[s->bp] = SWD_UINT(s->f + 1);
    }
    else
    {
        if (swd_search2(s))
            if (s->look >= 3)
                swd_search(s,node,cnt);
        if (s->m_len > len)
            s->m_off = swd_pos2off(s,s->m_pos);
        s->best3[s->bp] = SWD_UINT(s->m_len);

        if (s->use_best_off)
        {
            int i;
            for (i = 2; i < SWD_BEST_OFF; i++)
                if (s->best_pos[i] > 0)
                    s->best_off[i] = swd_pos2off(s,s->best_pos[i]-1);
                else
                    s->best_off[i] = 0;
        }
    }

    swd_remove_node(s,s->rp);

    /* add bp into HEAD2 */
    key = HEAD2(s->b,s->bp);
    s->head2[key] = SWD_UINT(s->bp);
}

int main()
{
    lzo_swd_p s = (lzo_swd_p)calloc(1, sizeof(lzo1x_999_swd_t));
    if (!s) { std::printf("00c07420_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 65536; k++) s->head2[k] = NIL2;
    for (int k = 0; k < 120; k++) s->b[k] = (lzo_byte)("abc"[k % 3]);
    s->b_size = SWD_N; s->f = SWD_F; s->max_chain = SWD_MAX_CHAIN; s->nice_length = SWD_F;
    s->use_best_off = 1; s->node_count = 1000; s->rp = 0;
    int fail = 0;
    for (lzo_uint bp = 0; bp < 20; bp++)
    {
        s->bp = bp; s->look = 18; s->m_len = 1;
        memset(s->best_pos, 0, sizeof(s->best_pos));
        lzo_uint before = s->node_count;
        swd_findbest(s);
        if (s->node_count != before - 1) fail |= 1;
        if (s->head2[HEAD2(s->b, bp)] != bp) fail |= 2;
        if (s->b_char != 'a' + (int)(bp % 3)) fail |= 4;
        if (bp < 3)
        {
            if (bp == 0 && s->m_len != 1) fail |= 8;
            continue;
        }
        if (s->m_len != 18) fail |= 16;
        if (s->m_off != 3) fail |= 32;
        if (s->best3[bp] != 18) fail |= 64;
        if (s->best_off[2] != 3) fail |= 128;
    }
    /* look reaches m_len: the early arm */
    s->bp = 30; s->look = 0; s->m_len = 1; s->m_off = 77;
    swd_findbest(s);
    if (s->b_char != -1 || s->m_off != 0 || s->best3[30] != SWD_F + 1) fail |= 256;
    free(s);
    if (fail) { std::printf("00c07420_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c07420_TEST PASS\n");
    return 0;
}
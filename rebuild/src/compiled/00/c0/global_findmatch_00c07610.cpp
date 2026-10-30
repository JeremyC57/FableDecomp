// _global::find_match(lzo1x_999_t* c, lzo1x_999_swd_t* s, lzo_uint this_len, lzo_uint skip) -- LZO 1.08 lzo1x_9x.c find_match: accept the previous match, search the window for the best match at the new position, fetch the next byte and report progress through the callback
// Shape (retry1): parked as "LTCG custom register convention"; find_match and its three callees are TU-local statics, so the TU holds
//   them all inside `extern "C" { static __declspec(noinline) }` with a same-TU caller and VC7.1 /O2 picks retail's private conventions
//   itself (chunk 585/586/593 lever): find_match c in EBX, s in ECX, this_len in EAX, skip on the stack, plain ret; swd_accept s in EAX,
//   n in ECX; swd_findbest s in ESI; swd_getbyte s in EAX. swd_remove_node must stay __forceinline (retail inlines it into swd_accept);
//   as a plain or noinline static it changes swd_accept's convention to s in ESI, n in EAX. (Retail swd_findbest calls the out-of-line
//   copy 0x00C07140; only find_match is graded here.)
// Leaf: _find_match
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C07610, 166 bytes. LZO 1.08 lzo1x_9x.c (includes lzo_swd.ch); callees swd_accept 0x00C071B0,
//   swd_findbest 0x00C07420, swd_getbyte 0x00C07090; the inline memset of best_pos is `rep stosd` (0x22 dwords).
// Layout: lzo1x_999_t / lzo1x_999_swd_t as the landed swd_remove_node 0x00C07140 (Ego_r PDB layouts).
// Layout: lzo1x_999_t / lzo1x_999_swd_t as the landed swd_remove_node 0x00C07140 (Ego_r PDB layouts).
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef unsigned long  lzo_uint32;
typedef unsigned short swd_uint;
typedef void (__cdecl *lzo_progress_callback_t)(lzo_uint, lzo_uint);

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
#include <string.h>
#define lzo_memset memset
#define swd_end(s)

extern "C" {

static __declspec(noinline) void swd_getbyte(lzo_swd_p s)
{
    int c;

    if ((c = getbyte(*(s->c))) < 0)
    {
        if (s->look > 0)
            --s->look;
    }
    else
    {
        s->b[s->ip] = LZO_BYTE(c);
        if (s->ip < s->f)
            s->b_wrap[s->ip] = LZO_BYTE(c);
    }
    if (++s->ip == s->b_size)
        s->ip = 0;
    if (++s->bp == s->b_size)
        s->bp = 0;
    if (++s->rp == s->b_size)
        s->rp = 0;
}

static __forceinline void swd_remove_node(lzo_swd_p s, lzo_uint node)
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

static __declspec(noinline) void swd_accept(lzo_swd_p s, lzo_uint n)
{
    if (n == 0)
        return;
    do
    {
        lzo_uint key;

        swd_remove_node(s,s->rp);

        /* add bp into HEAD3 */
        key = HEAD3(s->b,s->bp);
        s->succ3[s->bp] = s_get_head3(s,key);
        s->head3[key] = SWD_UINT(s->bp);
        s->best3[s->bp] = SWD_UINT(s->f + 1);
        s->llen3[key]++;

        /* add bp into HEAD2 */
        key = HEAD2(s->b,s->bp);
        s->head2[key] = SWD_UINT(s->bp);

        swd_getbyte(s);
    } while (--n != 0);
}

static __declspec(noinline) void swd_search(lzo_swd_p s, lzo_uint node, lzo_uint cnt)
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

static __declspec(noinline) lzo_bool swd_search2(lzo_swd_p s)
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

static __declspec(noinline) void swd_findbest(lzo_swd_p s)
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

static __declspec(noinline) int find_match(lzo1x_999_t* c, lzo_swd_p s, lzo_uint this_len, lzo_uint skip)
{
    if (skip > 0)
    {
        swd_accept(s, this_len - skip);
        c->textsize += this_len - skip + 1;
    }
    else
    {
        c->textsize += this_len - skip;
    }

    s->m_len = SWD_THRESHOLD;
    if (s->use_best_off)
        lzo_memset(s->best_pos,0,sizeof(s->best_pos));
    swd_findbest(s);
    c->m_len = s->m_len;
    c->m_off = s->m_off;

    swd_getbyte(s);

    if (s->b_char < 0)
    {
        c->look = 0;
        c->m_len = 0;
        swd_end(s);
    }
    else
    {
        c->look = s->look + 1;
    }
    c->bp = c->ip - c->look;

    if (c->cb && c->textsize > c->printcount)
    {
        (*c->cb)(c->textsize,c->codesize);
        c->printcount += 1024;
    }

    return LZO_E_OK;
}

// Same-TU caller (the find_match calls of lzo1x_9x.c lzo1x_999_compress_internal), present so the static gets retail's
// private convention. Not graded.
int __cdecl lzo1x_find_match_caller(lzo1x_999_t* c, lzo_swd_p s, lzo_uint this_len, lzo_uint skip) { return find_match(c, s, this_len, skip); }
}
// swd_accept(lzo_swd_p s, lzo_uint n) -- LZO 1.08 lzo_swd.ch swd_accept: accept n bytes of the look-ahead, unlinking the oldest node and inserting each position into the 3-byte and 2-byte hash heads before fetching the next byte
// Grade: DIFFER(279v279) -- prologue only: retail tests n (`test ecx,ecx ; je ret`) before `push ebx ; push esi ; mov esi,ecx`; VC7.1 here emits `push esi ; mov esi,ecx ; test esi,esi ; je ; push ebx` (and the matching epilogue). Body from 0x00C071C0 to the call is identical.
// Leaf: _swd_accept
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C071B0, 279 bytes. LZO 1.08 (lzo1x_9x.c includes lzo_swd.ch); the neighbours
// swd_getbyte 0x00C07090, swd_remove_node 0x00C07140 (landed) and the caller find_match 0x00C07610 (call at 0x00C07625).
// Convention: swd_accept is a `static` with a same-TU caller, so VC7.1 gives it a private register convention: s in eax,
// n in ecx, plain `ret` (chunk 585/586 rule; extern "C" { static __declspec(noinline) } keeps the C name). Its one call is
// swd_getbyte 0x00C07090, also a same-TU static taking s in eax -- its verbatim body sits above. swd_remove_node is inlined
// here (it also has an out-of-line copy at 0x00C07140 for its other callers).
// Shape: `if (n) do { ... } while (--n != 0);` gives the `test ecx,ecx ; je ret` guard before the pushes and `dec esi ; jne`.
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

extern "C" void __fastcall swd_findbest(lzo_swd_p s);   // 0x00C07420 (not graded; a stand-in declaration for the caller)

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
    if (n) do
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

}

// Same-TU caller (the shape of lzo1x_9x.c find_match), present so swd_accept gets retail's private convention.
extern "C" int __cdecl find_match(lzo1x_999_t* c, lzo_swd_p s, lzo_uint this_len, lzo_uint skip)
{
    if (skip > 0)
    {
        swd_accept(s, this_len - skip);
        c->textsize += this_len - skip + 1;
    }
    else
        c->textsize += this_len - skip;
    s->m_len = THRESHOLD;
    swd_findbest(s);
    c->m_len = s->m_len;
    c->m_off = s->m_off;
    swd_getbyte(s);
    return 0;
}

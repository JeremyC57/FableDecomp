// _global::STORE_RUN(lzo1x_999_t* c, lzo_byte* op, const lzo_byte* ii, lzo_uint t) -- LZO 1.08 lzo1x_9x.c STORE_RUN: emit a literal run of t bytes (first-run, short, medium or long length encoding) and copy the literals
// Shape (retry1): parked as "LTCG custom register convention"; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the LZO static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _STORE_RUN
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C07890, 191 bytes. Retail convention (VC7.1 private, from the same-TU static): op in ECX, c / ii / t on the stack, plain ret (caller cleans).
// The library sources are zlib 1.2.1 (ZLIB_VERSION at 0x0129CFE0, Z_PREFIX build) and LZO 1.08 (lzo1x_9x.c + lzo_swd.ch); layouts
// from the Ego_r PDB.
// ---- LZO 1.08 lzo1x_9x.c / lzo_swd.ch model (layouts from the Ego_r PDB: lzo1x_999_t 116, lzo1x_999_swd_t 455016) ----
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

#define LZO_BYTE(x) ((unsigned char) (x))
#define M1_MAX_OFFSET 0x0400
#define M2_MIN_LEN 3
#define M2_MAX_LEN 8
#define M2_MAX_OFFSET 0x0800
#define MX_MAX_OFFSET (M1_MAX_OFFSET+M2_MAX_OFFSET)
#define M3_MAX_LEN 33
#define M3_MAX_OFFSET 0x4000
#define M4_MAX_LEN 9
#define M4_MAX_OFFSET 0xbfff
#define SWD_THRESHOLD 1
#define LZO_E_OK 0
#define getbyte(c)  ((c).ip < (c).in_end ? *((c).ip)++ : (-1))

extern "C" {
static __declspec(noinline) lzo_byte* STORE_RUN(lzo1x_999_t* c, lzo_byte* op, const lzo_byte* ii, lzo_uint t)
{
    c->lit_bytes += t;

    if (op == c->out && t <= 238)
    {
        *op++ = LZO_BYTE(17 + t);
    }
    else if (t <= 3)
    {
        op[-2] |= LZO_BYTE(t);
        c->lit1_r++;
    }
    else if (t <= 18)
    {
        *op++ = LZO_BYTE(t - 3);
        c->lit2_r++;
    }
    else
    {
        lzo_uint tt = t - 18;

        *op++ = 0;
        while (tt > 255)
        {
            tt -= 255;
            *op++ = 0;
        }
        *op++ = LZO_BYTE(tt);
        c->lit3_r++;
    }
    do *op++ = *ii++; while (--t > 0);

    return op;
}

// Same-TU caller (the STORE_RUN calls of lzo1x_9x.c lzo1x_999_compress_internal / code_run), present so the static gets retail's private
// convention. Not graded.
lzo_byte* __cdecl lzo1x_STORE_RUN_caller(lzo1x_999_t* c, lzo_byte* op, const lzo_byte* ii, lzo_uint t) { return STORE_RUN(c, op, ii, t); }
}
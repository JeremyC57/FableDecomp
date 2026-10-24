// _global::swd_search(lzo_swd_p s, lzo_uint node, lzo_uint cnt) -- LZO 1.08 lzo_swd.ch swd_search: walk cnt nodes of the 3-byte hash chain from node, recording the longest match (and the best position per length) at the current buffer position
// Grade: DIFFER(231v232) -- only the convention: retail takes s in eax and node in ecx, cnt on the stack (LTCG); lane gap: LTCG custom register convention (not reproducible from one VC7.1 TU)
// Leaf: swd_search
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C072D0, 232 bytes. Reference body only: retail (LTCG custom convention): s in EAX, node in ECX, cnt on the stack, plain ret.
// The library sources are zlib 1.2.1 (ZLIB_VERSION at 0x0129CFE0, Z_PREFIX build) and LZO 1.08 (lzo1x_9x.c + lzo_swd.ch); layouts
// from the Ego_r PDB. Written as an ordinary __cdecl function so the TU emits it; the lane cannot reproduce the
// whole-program register convention, so this row is parked with this body as the reference.
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

#define SWD_BEST_OFF 34
extern "C" void __cdecl swd_search(lzo_swd_p s, lzo_uint node, lzo_uint cnt)
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

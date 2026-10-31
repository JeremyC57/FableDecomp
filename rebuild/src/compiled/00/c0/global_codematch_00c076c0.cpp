// _global::code_match(lzo1x_999_t* c, lzo_byte* op, lzo_uint m_len, lzo_uint m_off) -- LZO 1.08 lzo1x_9x.c code_match: emit one M1/M2/M3/M4 match code for (m_len, m_off), count it and remember it as the last match
// Leaf: _code_match
// Shape: the LZO 1.08 lzo1x_9x.c body verbatim (LZO1X arms, asserts compiled out), a TU-local static inside
//   `extern "C" { static __declspec(noinline) }` with a same-TU caller (chunk 585/601 lever): VC7.1 /O2 gives it retail's
//   private convention by itself (m_len in EAX, op in ECX, c and m_off on the stack, caller pops). The two
//   `while (m_len > 255)` zero runs compile to retail's rep stosd/stosb plus the 255-step count loop.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C076C0, 451 bytes; no relocations. Statistics m1a_m +0x54, m1b_m +0x58, m2_m +0x5c,
//   m3_m +0x60, m4_m +0x64; r1_lit +0x4c; match_bytes +0x40; last_m_len/last_m_off +0x10/+0x14.
// Layout: lzo1x_999_t (LZO_COMPRESS_T) as the landed find_match 0x00C07610 / swd_remove_node 0x00C07140.
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef void (__cdecl *lzo_progress_callback_t)(lzo_uint, lzo_uint);

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
#define LZO_COMPRESS_T lzo1x_999_t

#define LZO_BYTE(x)     ((unsigned char) (x))
#define M1_MAX_OFFSET   0x0400
#define M2_MAX_OFFSET   0x0800
#define M3_MAX_OFFSET   0x4000
#define M2_MIN_LEN      3
#define M2_MAX_LEN      8
#define M3_MAX_LEN      33
#define M4_MAX_LEN      9
#define M1_MARKER       0
#define M3_MARKER       32
#define M4_MARKER       16
#define MX_MAX_OFFSET   (M1_MAX_OFFSET + M2_MAX_OFFSET)

extern "C" {

static __declspec(noinline) lzo_byte *
code_match ( LZO_COMPRESS_T *c, lzo_byte *op, lzo_uint m_len, lzo_uint m_off )
{
    lzo_uint x_len = m_len;
    lzo_uint x_off = m_off;

    c->match_bytes += m_len;

    if (m_len == 2)
    {
        m_off -= 1;
        *op++ = LZO_BYTE(M1_MARKER | ((m_off & 3) << 2));
        *op++ = LZO_BYTE(m_off >> 2);
        c->m1a_m++;
    }
    else if (m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET)
    {
        m_off -= 1;
        *op++ = LZO_BYTE(((m_len - 1) << 5) | ((m_off & 7) << 2));
        *op++ = LZO_BYTE(m_off >> 3);
        c->m2_m++;
    }
    else if (m_len == M2_MIN_LEN && m_off <= MX_MAX_OFFSET && c->r1_lit >= 4)
    {
        m_off -= 1 + M2_MAX_OFFSET;
        *op++ = LZO_BYTE(M1_MARKER | ((m_off & 3) << 2));
        *op++ = LZO_BYTE(m_off >> 2);
        c->m1b_m++;
    }
    else if (m_off <= M3_MAX_OFFSET)
    {
        m_off -= 1;
        if (m_len <= M3_MAX_LEN)
            *op++ = LZO_BYTE(M3_MARKER | (m_len - 2));
        else
        {
            m_len -= M3_MAX_LEN;
            *op++ = M3_MARKER | 0;
            while (m_len > 255)
            {
                m_len -= 255;
                *op++ = 0;
            }
            *op++ = LZO_BYTE(m_len);
        }
        *op++ = LZO_BYTE(m_off << 2);
        *op++ = LZO_BYTE(m_off >> 6);
        c->m3_m++;
    }
    else
    {
        lzo_uint k;

        m_off -= 0x4000;
        k = (m_off & 0x4000) >> 11;
        if (m_len <= M4_MAX_LEN)
            *op++ = LZO_BYTE(M4_MARKER | k | (m_len - 2));
        else
        {
            m_len -= M4_MAX_LEN;
            *op++ = LZO_BYTE(M4_MARKER | k | 0);
            while (m_len > 255)
            {
                m_len -= 255;
                *op++ = 0;
            }
            *op++ = LZO_BYTE(m_len);
        }
        *op++ = LZO_BYTE(m_off << 2);
        *op++ = LZO_BYTE(m_off >> 6);
        c->m4_m++;
    }

    c->last_m_len = x_len;
    c->last_m_off = x_off;
    return op;
}

// Same-TU caller (lzo1x_999_compress_internal's `op = code_match(c,op,m_len,m_off);`), present so the static gets retail's
// private convention. Not graded.
lzo_byte* __cdecl lzo1x_code_match_caller(LZO_COMPRESS_T* c, lzo_byte* op, lzo_uint m_len, lzo_uint m_off) { return code_match(c, op, m_len, m_off); }
}
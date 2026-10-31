// Behaviour test for 0x00C076C0 code_match (LZO 1.08 lzo1x_9x.c): a copy of the model and the upstream body, checked
// against hand-encoded LZO1X match codes for every arm (M1a, M2, M1b, M3 short/long, M4 short/long).
#include <cstdio>
#include <cstring>
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef void (*lzo_progress_callback_t)(lzo_uint, lzo_uint);
typedef struct
{
    int init; lzo_uint look; lzo_uint m_len; lzo_uint m_off; lzo_uint last_m_len; lzo_uint last_m_off;
    const lzo_byte* bp; const lzo_byte* ip; const lzo_byte* in; const lzo_byte* in_end; lzo_byte* out;
    lzo_progress_callback_t cb; lzo_uint textsize; lzo_uint codesize; lzo_uint printcount;
    unsigned long lit_bytes; unsigned long match_bytes; unsigned long rep_bytes; unsigned long lazy;
    lzo_uint r1_lit; lzo_uint r1_m_len;
    unsigned long m1a_m, m1b_m, m2_m, m3_m, m4_m;
    unsigned long lit1_r, lit2_r, lit3_r;
} lzo1x_999_t;
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

static lzo_byte* code_match(LZO_COMPRESS_T* c, lzo_byte* op, lzo_uint m_len, lzo_uint m_off)
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
            while (m_len > 255) { m_len -= 255; *op++ = 0; }
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
            while (m_len > 255) { m_len -= 255; *op++ = 0; }
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

static int check(lzo1x_999_t* c, lzo_uint len, lzo_uint off, const unsigned char* want, int n)
{
    lzo_byte buf[1200];
    memset(buf, 0xAA, sizeof(buf));
    lzo_byte* end = code_match(c, buf, len, off);
    if (end - buf != n) return 0;
    if (memcmp(buf, want, n) != 0) return 0;
    if (c->last_m_len != len || c->last_m_off != off) return 0;
    return 1;
}

int main()
{
    lzo1x_999_t c;
    memset(&c, 0, sizeof(c));
    int fail = 0;
    c.r1_lit = 1;
    { unsigned char w[] = { 0x0C, 0x40 };        if (!check(&c, 2, 0x104, w, 2)) fail |= 1; }     /* M1a: off-1 = 0x103 */
    { unsigned char w[] = { 0xBC, 0xFF };        if (!check(&c, 6, 0x800, w, 2)) fail |= 2; }     /* M2: (5<<5)|(7<<2), 0x7ff>>3 */
    c.r1_lit = 4;
    { unsigned char w[] = { 0x0C, 0x40 };        if (!check(&c, 3, 0x904, w, 2)) fail |= 4; }     /* M1b: off-0x801 = 0x103 */
    c.r1_lit = 1;
    { unsigned char w[] = { 0x28, 0xFC, 0x03 };  if (!check(&c, 10, 0x100, w, 3)) fail |= 8; }    /* M3 short */
    {
        unsigned char w[8];
        int n = 0; w[n++] = 0x20; w[n++] = 0; w[n++] = 300 - 33 - 255; w[n++] = 0xFC; w[n++] = 0x03;
        if (!check(&c, 300, 0x100, w, n)) fail |= 16;                                              /* M3 long, one zero */
    }
    { unsigned char w[] = { 0x13, 0x08, 0x00 };  if (!check(&c, 5, 0x4002, w, 3)) fail |= 32; }   /* M4 short, k=0 */
    { unsigned char w[] = { 0x1D, 0x00, 0x00 };  if (!check(&c, 7, 0x8000, w, 3)) fail |= 64; }   /* M4 short, k=8 */
    {
        lzo_byte buf[1200];
        lzo_uint len = 9 + 3 * 255 + 7;
        lzo_byte* end = code_match(&c, buf, len, 0x5000);
        /* marker 0x10, three zero bytes, 7, then off-0x4000 = 0x1000 -> 0x00, 0x40 */
        unsigned char w[] = { 0x10, 0, 0, 0, 7, 0x00, 0x40 };
        if (end - buf != 7 || memcmp(buf, w, 7) != 0) fail |= 128;
    }
    if (c.m1a_m != 1 || c.m2_m != 1 || c.m1b_m != 1 || c.m3_m != 2 || c.m4_m != 3) fail |= 256;
    if (c.match_bytes != 2 + 6 + 3 + 10 + 300 + 5 + 7 + (9 + 3 * 255 + 7)) fail |= 512;
    if (fail) { std::printf("00c076c0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c076c0_TEST PASS\n");
    return 0;
}
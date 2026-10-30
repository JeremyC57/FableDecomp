// Behaviour test for 00c06de0 (adapted from the landed host row 0x00c06f30's test, whose TU exercises this function through its real caller).
// Behaviour test for 0x00C06F30 swd_init (LZO 1.08 lzo_swd.ch): window defaults, cleared tables, dictionary load,
// look-ahead priming from the input and dictionary insertion.
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef unsigned int  lzo_uint32;
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
    unsigned int lit_bytes;   // +0x3c
    unsigned int match_bytes; // +0x40
    unsigned int rep_bytes;   // +0x44
    unsigned int lazy;        // +0x48
    lzo_uint r1_lit;           // +0x4c
    lzo_uint r1_m_len;         // +0x50
    unsigned int m1a_m, m1b_m, m2_m, m3_m, m4_m;   // +0x54..+0x64
    unsigned int lit1_r, lit2_r, lit3_r;           // +0x68..+0x70
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

#define SWD_THRESHOLD  1
#define LZO_E_OK       0
#define LZO_E_ERROR    (-1)
#define SWD_UINT(x) ((swd_uint)(x))
#define s_get_head3(s,key) s->head3[key]

#define lzo_memcpy std::memcpy
#define lzo_memset std::memset

static void swd_initdict(lzo_swd_p s, const lzo_byte* dict, lzo_uint dict_len)
{                                                                                              // 0x00C06DE0
    s->dict = s->dict_end = 0;
    s->dict_len = 0;

    if (!dict || dict_len <= 0)
        return;
    if (dict_len > s->n)
    {
        dict += dict_len - s->n;
        dict_len = s->n;
    }

    s->dict = dict;
    s->dict_len = dict_len;
    s->dict_end = dict + dict_len;
    lzo_memcpy(s->b,dict,dict_len);
    s->ip = dict_len;
}

static void swd_insertdict(lzo_swd_p s, lzo_uint node, lzo_uint len)
{                                                                                              // 0x00C06E50
    lzo_uint key;

    s->node_count = s->n - len;
    s->first_rp = node;

    while (len-- > 0)
    {
        key = HEAD3(s->b,node);
        s->succ3[node] = s_get_head3(s,key);
        s->head3[key] = SWD_UINT(node);
        s->best3[node] = SWD_UINT(s->f + 1);
        s->llen3[key]++;

        key = HEAD2(s->b,node);
        s->head2[key] = SWD_UINT(node);

        node++;
    }
}

static int swd_init(lzo_swd_p s, const lzo_byte* dict, lzo_uint dict_len)
{
    s->n = SWD_N;
    s->f = SWD_F;
    s->threshold = SWD_THRESHOLD;

    s->max_chain = SWD_MAX_CHAIN;
    s->nice_length = SWD_F;
    s->use_best_off = 0;
    s->lazy_insert = 0;

    s->b_size = s->n + s->f;
    if (2 * s->f >= s->n || s->b_size + s->f >= NIL2)
        return LZO_E_ERROR;
    s->b_wrap = s->b + s->b_size;
    s->node_count = s->n;

    lzo_memset(s->llen3, 0, sizeof(s->llen3[0]) * SWD_HSIZE);
    lzo_memset(s->head2, 0xff, sizeof(s->head2[0]) * 65536L);

    s->ip = 0;
    swd_initdict(s,dict,dict_len);
    s->bp = s->ip;
    s->first_rp = s->ip;

    s->look = (lzo_uint) (s->c->in_end - s->c->ip);
    if (s->look > 0)
    {
        if (s->look > s->f)
            s->look = s->f;
        lzo_memcpy(&s->b[s->ip],s->c->ip,s->look);
        s->c->ip += s->look;
        s->ip += s->look;
    }
    if (s->ip == s->b_size)
        s->ip = 0;

    if (s->look >= 2 && s->dict_len > 0)
        swd_insertdict(s,0,s->dict_len);

    s->rp = s->first_rp;
    if (s->rp >= s->node_count)
        s->rp -= s->node_count;
    else
        s->rp += s->b_size - s->node_count;

    return LZO_E_OK;
}

static int init_match(lzo1x_999_t* c, lzo_swd_p s, const lzo_byte* dict, lzo_uint dict_len,
                                           lzo_uint32 flags)
{                                                                                              // 0x00C075C0
    int r;

    c->init = 1;

    s->c = c;

    c->last_m_len = c->last_m_off = 0;

    c->textsize = c->codesize = c->printcount = 0;
    c->lit_bytes = c->match_bytes = c->rep_bytes = 0;
    c->lazy = 0;

    r = swd_init(s,dict,dict_len);
    if (r != 0)
        return r;

    s->use_best_off = (flags & 1) ? 1 : 0;
    return r;
}

int main()
{
    int fail = 0;
    lzo1x_999_swd_t* s = (lzo1x_999_swd_t*)std::malloc(sizeof(lzo1x_999_swd_t));
    std::memset(s, 0x5a, sizeof(*s));
    lzo1x_999_t c; std::memset(&c, 0, sizeof(c));
    static lzo_byte input[5000];
    for (int i = 0; i < 5000; ++i) input[i] = (lzo_byte)(i * 7);
    c.ip = input; c.in = input; c.in_end = input + 5000;
    lzo_byte dict[4] = { 1, 2, 3, 4 };
    int r = init_match(&c, s, dict, 4, 1);
    if (r != 0 || c.init != 1 || s->c != &c || s->use_best_off != 1) fail = 1;
    if (s->n != 0xbfff || s->f != 0x800 || s->threshold != 1 || s->max_chain != 0x800 || s->nice_length != 0x800) fail = 2;
    if (s->b_size != 0xc7ff || s->b_wrap != s->b + 0xc7ff || s->lazy_insert != 0) fail = 3;
    if (s->dict_len != 4 || s->dict != dict || s->dict_end != dict + 4 || s->b[0] != 1 || s->b[3] != 4) fail = 4;
    if (s->look != 0x800 || c.ip != input + 0x800 || s->ip != 4 + 0x800 || s->bp != 4 || s->b[4] != input[0]) fail = 5;
    if (s->node_count != 0xbfff - 4 || s->first_rp != 0 || s->rp != 0 + 0xc7ff - (0xbfff - 4)) fail = 6;
    if (s->head2[0x0201] != 0 || s->head2[0x1234] != 0xffff) fail = 7;
    if (s->llen3[HEAD3(s->b, 0)] == 0 || s->best3[0] != 0x801) fail = 8;
    /* no dictionary, short input */
    c.ip = input; c.in_end = input + 10;
    r = swd_init(s, 0, 0);
    if (r != 0 || s->dict_len != 0 || s->look != 10 || s->ip != 10 || s->node_count != 0xbfff || s->rp != 0 + 0xc7ff - 0xbfff) fail = 9;
    std::free(s);
    if (fail) { std::printf("00c06de0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c06de0_TEST PASS\n");
    return 0;
}
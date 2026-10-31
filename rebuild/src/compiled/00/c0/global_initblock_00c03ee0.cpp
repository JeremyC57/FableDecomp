// _global::init_block(deflate_state* s) -- zlib 1.2.1 trees.c init_block (static): zero the literal/length, distance and bit-length tree frequencies, seed END_BLOCK with 1 and clear the block statistics
// Leaf: _init_block
// Grade: MATCH (wave-2 sec6: the dummy-ecx __fastcall stand-in replaced by the trees.c static as a same-TU extern "C"
//   static __declspec(noinline) with its real caller _tr_init 0x00C051C0 in the TU; length from the retail bytes,
//   BOUNDS_FIX/oracle_extra)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C03EE0, 100 bytes. trees.c static, s in EDX (the private convention VC7.1 picks for a same-TU static), plain ret. No callees.
// TU: init_block as extern "C" static __declspec(noinline) with its real caller _tr_init 0x00C051C0 (trees.c 1.2.1 body; static_l_desc/static_d_desc/static_bl_desc extern). Land the _init_block leaf only.
// ---- zlib 1.2.1 model (zconf.h/zlib.h/deflate.h, Z_PREFIX build; layouts from the Ego_r PDB) ----
typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;
typedef ush            Pos;
typedef Pos            Posf;
typedef unsigned       IPos;
typedef void*          voidpf;
typedef voidpf (__cdecl *alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (__cdecl *free_func)(voidpf opaque, voidpf address);

struct internal_state;
typedef struct z_stream_s {
    Bytef*    next_in;      // +0x00
    uInt      avail_in;     // +0x04
    uLong     total_in;     // +0x08
    Bytef*    next_out;     // +0x0c
    uInt      avail_out;    // +0x10
    uLong     total_out;    // +0x14
    char*     msg;          // +0x18
    struct internal_state* state; // +0x1c
    alloc_func zalloc;      // +0x20
    free_func  zfree;       // +0x24
    voidpf     opaque;      // +0x28
    int        data_type;   // +0x2c
    uLong      adler;       // +0x30
    uLong      reserved;    // +0x34
} z_stream;                 // 0x38
typedef z_stream* z_streamp;
struct ct_data_s { union { ush freq; ush code; } fc; union { ush dad; ush len; } dl; };
typedef struct ct_data_s ct_data;
struct static_tree_desc_s;
struct tree_desc_s { ct_data* dyn_tree; int max_code; struct static_tree_desc_s* stat_desc; };

typedef struct internal_state {
    z_streamp strm;            // +0x00
    int    status;             // +0x04
    Bytef* pending_buf;        // +0x08
    ulg    pending_buf_size;   // +0x0c
    Bytef* pending_out;        // +0x10
    int    pending;            // +0x14
    int    wrap;               // +0x18
    Byte   data_type;          // +0x1c
    Byte   method;             // +0x1d
    int    last_flush;         // +0x20
    uInt   w_size;             // +0x24
    uInt   w_bits;             // +0x28
    uInt   w_mask;             // +0x2c
    Bytef* window;             // +0x30
    ulg    window_size;        // +0x34
    Posf*  prev;               // +0x38
    Posf*  head;               // +0x3c
    uInt   ins_h;              // +0x40
    uInt   hash_size;          // +0x44
    uInt   hash_bits;          // +0x48
    uInt   hash_mask;          // +0x4c
    uInt   hash_shift;         // +0x50
    long   block_start;        // +0x54
    uInt   match_length;       // +0x58
    IPos   prev_match;         // +0x5c
    int    match_available;    // +0x60
    uInt   strstart;           // +0x64
    uInt   match_start;        // +0x68
    uInt   lookahead;          // +0x6c
    uInt   prev_length;        // +0x70
    uInt   max_chain_length;   // +0x74
    uInt   max_lazy_match;     // +0x78
    int    level;              // +0x7c
    int    strategy;           // +0x80
    uInt   good_match;         // +0x84
    int    nice_match;         // +0x88
    struct ct_data_s dyn_ltree[573];   // +0x8c
    struct ct_data_s dyn_dtree[61];    // +0x980
    struct ct_data_s bl_tree[39];      // +0xa74
    struct tree_desc_s l_desc;         // +0xb10
    struct tree_desc_s d_desc;         // +0xb1c
    struct tree_desc_s bl_desc;        // +0xb28
    ush    bl_count[16];               // +0xb34
    int    heap[573];                  // +0xb54
    int    heap_len;                   // +0x1448
    int    heap_max;                   // +0x144c
    uch    depth[573];                 // +0x1450
    uch*   l_buf;                      // +0x1690
    uInt   lit_bufsize;                // +0x1694
    uInt   last_lit;                   // +0x1698
    ush*   d_buf;                      // +0x169c
    ulg    opt_len;                    // +0x16a0
    ulg    static_len;                 // +0x16a4
    uInt   matches;                    // +0x16a8
    int    last_eob_len;               // +0x16ac
    ush    bi_buf;                     // +0x16b0
    int    bi_valid;                   // +0x16b4
} deflate_state;                       // 0x16b8

#define Freq fc.freq
#define L_CODES 286
#define D_CODES 30
#define BL_CODES 19
#define END_BLOCK 256
struct static_tree_desc_s { const ct_data* static_tree; const int* extra_bits; int extra_base; int elems; int max_length; };
typedef struct static_tree_desc_s static_tree_desc;
extern "C" static_tree_desc static_l_desc;    // trees.c local
extern "C" static_tree_desc static_d_desc;
extern "C" static_tree_desc static_bl_desc;

extern "C" {
static __declspec(noinline) void init_block(deflate_state* s)                  // 0x00C03EE0 (trees.c static)
{
    int n; /* iterates over tree elements */

    /* Initialize the trees. */
    for (n = 0; n < L_CODES;  n++) s->dyn_ltree[n].Freq = 0;
    for (n = 0; n < D_CODES;  n++) s->dyn_dtree[n].Freq = 0;
    for (n = 0; n < BL_CODES; n++) s->bl_tree[n].Freq = 0;

    s->dyn_ltree[END_BLOCK].Freq = 1;
    s->opt_len = s->static_len = 0L;
    s->last_lit = s->matches = 0;
}
}

extern "C" void __cdecl _tr_init(deflate_state* s)                              // 0x00C051C0 (same-TU caller)
{
    s->l_desc.dyn_tree = s->dyn_ltree;
    s->l_desc.stat_desc = &static_l_desc;

    s->d_desc.dyn_tree = s->dyn_dtree;
    s->d_desc.stat_desc = &static_d_desc;

    s->bl_desc.dyn_tree = s->bl_tree;
    s->bl_desc.stat_desc = &static_bl_desc;

    s->bi_buf = 0;
    s->bi_valid = 0;
    s->last_eob_len = 8; /* enough lookahead for inflate */

    /* Initialize the first block of the first file: */
    init_block(s);
}
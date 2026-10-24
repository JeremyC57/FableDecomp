#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define CDECL __cdecl
#define FASTCALL __fastcall
#else
#define CDECL
#define FASTCALL
#endif
typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned int   uLong;   // 32-bit on the retail target; fixed width for the 64-bit host build
typedef Byte           Bytef;
typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned int   ulg;
typedef ush            Pos;
typedef Pos            Posf;
typedef unsigned       IPos;
typedef void*          voidpf;
typedef voidpf (CDECL *alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (CDECL *free_func)(voidpf opaque, voidpf address);
struct internal_state;
typedef struct z_stream_s {
    Bytef* next_in; uInt avail_in; uLong total_in;
    Bytef* next_out; uInt avail_out; uLong total_out;
    char* msg; struct internal_state* state;
    alloc_func zalloc; free_func zfree; voidpf opaque;
    int data_type; uLong adler; uLong reserved;
} z_stream;
typedef z_stream* z_streamp;
struct ct_data_s { union { ush freq; ush code; } fc; union { ush dad; ush len; } dl; };
typedef struct ct_data_s ct_data;
struct static_tree_desc_s;
struct tree_desc_s { ct_data* dyn_tree; int max_code; struct static_tree_desc_s* stat_desc; };
typedef struct internal_state {
    z_streamp strm; int status; Bytef* pending_buf; ulg pending_buf_size; Bytef* pending_out;
    int pending; int wrap; Byte data_type; Byte method; int last_flush;
    uInt w_size; uInt w_bits; uInt w_mask; Bytef* window; ulg window_size; Posf* prev; Posf* head;
    uInt ins_h; uInt hash_size; uInt hash_bits; uInt hash_mask; uInt hash_shift; long block_start;
    uInt match_length; IPos prev_match; int match_available; uInt strstart; uInt match_start;
    uInt lookahead; uInt prev_length; uInt max_chain_length; uInt max_lazy_match; int level;
    int strategy; uInt good_match; int nice_match;
    struct ct_data_s dyn_ltree[573]; struct ct_data_s dyn_dtree[61]; struct ct_data_s bl_tree[39];
    struct tree_desc_s l_desc; struct tree_desc_s d_desc; struct tree_desc_s bl_desc;
    ush bl_count[16]; int heap[573]; int heap_len; int heap_max; uch depth[573];
    uch* l_buf; uInt lit_bufsize; uInt last_lit; ush* d_buf; ulg opt_len; ulg static_len;
    uInt matches; int last_eob_len; ush bi_buf; int bi_valid;
} deflate_state;
#define Freq fc.freq
#define LITERALS 256
#define Z_BINARY 0
#define Z_ASCII 1
static void FASTCALL set_data_type(deflate_state* s)
{
    int n = 0; unsigned ascii_freq = 0; unsigned bin_freq = 0;
    while (n < 7)        bin_freq += s->dyn_ltree[n++].Freq;
    while (n < 128)    ascii_freq += s->dyn_ltree[n++].Freq;
    while (n < LITERALS) bin_freq += s->dyn_ltree[n++].Freq;
    s->data_type = (Byte)(bin_freq > (ascii_freq >> 2) ? Z_BINARY : Z_ASCII);
}
static deflate_state g_s;
int main()
{
    int bad = 0;
    g_s.dyn_ltree['a'].Freq = 400; g_s.dyn_ltree[3].Freq = 100; g_s.data_type = 9;
    set_data_type(&g_s); if (g_s.data_type != Z_ASCII) bad = 1;          // 100 > 400/4 is false
    g_s.dyn_ltree[200].Freq = 1; set_data_type(&g_s); if (g_s.data_type != Z_BINARY) bad = 2;   // 101 > 100
    g_s.dyn_ltree[256].Freq = 5000; g_s.dyn_ltree[200].Freq = 0; set_data_type(&g_s); if (g_s.data_type != Z_ASCII) bad = 3; // END_BLOCK ignored
    if (bad) { std::printf("00c04f90_TEST FAIL\n"); return 1; }
    std::printf("00c04f90_TEST PASS\n");
    return 0;
}
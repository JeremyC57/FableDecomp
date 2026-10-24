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
#define Z_OK            0
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define INIT_STATE    42
#define BUSY_STATE   113
#define FINISH_STATE 666
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))
#define TRY_FREE(s, p) {if (p) ZFREE(s, p);}

static int CDECL z_deflateEnd(z_streamp strm)
{
    int status;
    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    status = strm->state->status;
    if (status != INIT_STATE && status != BUSY_STATE && status != FINISH_STATE) return Z_STREAM_ERROR;
    TRY_FREE(strm, strm->state->pending_buf);
    TRY_FREE(strm, strm->state->head);
    TRY_FREE(strm, strm->state->prev);
    TRY_FREE(strm, strm->state->window);
    ZFREE(strm, strm->state);
    strm->state = 0;
    return status == BUSY_STATE ? Z_DATA_ERROR : Z_OK;
}

static void* g_freed[8]; static int g_nfreed; static void* g_opaque;
static void CDECL TestFree(voidpf opaque, voidpf p) { g_opaque = opaque; g_freed[g_nfreed++] = p; }
static deflate_state g_s;

static int Run(int status, int withBufs, int* nfreed)
{
    z_stream strm; std::memset(&strm, 0, sizeof(strm));
    std::memset(&g_s, 0, sizeof(g_s));
    static Bytef pb[4], win[4]; static Posf pr[4], hd[4];
    g_s.status = status;
    if (withBufs) { g_s.pending_buf = pb; g_s.window = win; g_s.prev = pr; g_s.head = hd; }
    strm.state = &g_s; strm.zfree = TestFree; strm.opaque = (voidpf)&strm;
    g_nfreed = 0;
    int r = z_deflateEnd(&strm);
    *nfreed = g_nfreed;
    if (r != Z_STREAM_ERROR && strm.state != 0) return 99;
    if (withBufs && r != Z_STREAM_ERROR) {
        if (g_freed[0] != pb || g_freed[1] != hd || g_freed[2] != pr || g_freed[3] != win || g_freed[4] != &g_s) return 98;
        if (g_opaque != (voidpf)&strm) return 97;
    }
    return r;
}

int main()
{
    int n = 0, bad = 0;
    if (z_deflateEnd(0) != Z_STREAM_ERROR) bad = 1;
    z_stream e; std::memset(&e, 0, sizeof(e));
    if (z_deflateEnd(&e) != Z_STREAM_ERROR) bad = 2;
    if (Run(INIT_STATE, 1, &n) != Z_OK || n != 5) bad = 3;
    if (Run(BUSY_STATE, 1, &n) != Z_DATA_ERROR || n != 5) bad = 4;
    if (Run(FINISH_STATE, 0, &n) != Z_OK || n != 1) bad = 5;
    if (Run(7, 1, &n) != Z_STREAM_ERROR || n != 0) bad = 6;
    if (bad) { std::printf("00c00ae0_TEST FAIL\n"); return 1; }
    std::printf("00c00ae0_TEST PASS\n");
    return 0;
}
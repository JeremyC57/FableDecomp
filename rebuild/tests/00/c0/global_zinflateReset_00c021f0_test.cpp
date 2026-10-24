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
typedef struct { unsigned char op; unsigned char bits; unsigned short val; } code;
typedef enum { HEAD = 0, FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC, DICTID, DICT,
    TYPE, TYPEDO, STORED, COPY, TABLE, LENLENS, CODELENS, LEN, LENEXT, DIST, DISTEXT,
    MATCH, LIT, CHECK, LENGTH, DONE, BAD, MEM, SYNC } inflate_mode;
struct inflate_state {
    inflate_mode mode; int last; int wrap; int havedict; int flags; unsigned int check; unsigned int total;
    unsigned wbits; unsigned wsize; unsigned whave; unsigned write; unsigned char* window;
    unsigned int hold; unsigned bits; unsigned length; unsigned offset; unsigned extra;
    code const* lencode; code const* distcode; unsigned lenbits; unsigned distbits;
    unsigned ncode; unsigned nlen; unsigned ndist; unsigned have; code* next;
    unsigned short lens[320]; unsigned short work[288]; code codes[1440];
};
#define Z_OK 0
#define Z_STREAM_ERROR (-2)
static int CDECL z_inflateReset(z_streamp strm)
{
    struct inflate_state* state;
    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = 0;
    state->mode = HEAD; state->last = 0; state->havedict = 0; state->wsize = 0; state->whave = 0;
    state->hold = 0; state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    return Z_OK;
}
static struct inflate_state g_st;
int main()
{
    int bad = 0;
    if (z_inflateReset(0) != Z_STREAM_ERROR) bad = 1;
    z_stream s; std::memset(&s, 0x5a, sizeof(s)); s.state = 0;
    if (z_inflateReset(&s) != Z_STREAM_ERROR) bad = 2;
    std::memset(&g_st, 0x5a, sizeof(g_st));
    s.state = (struct internal_state*)&g_st; s.msg = (char*)"x";
    if (z_inflateReset(&s) != Z_OK) bad = 3;
    if (s.total_in || s.total_out || s.msg || g_st.total || g_st.mode != HEAD || g_st.last || g_st.havedict ||
        g_st.wsize || g_st.whave || g_st.hold || g_st.bits) bad = 4;
    if (g_st.lencode != g_st.codes || g_st.distcode != g_st.codes || g_st.next != g_st.codes) bad = 5;
    if (g_st.wbits != 0x5a5a5a5au || g_st.write != 0x5a5a5a5au || g_st.wrap != 0x5a5a5a5a) bad = 6;   // untouched in 1.2.1
    if (bad) { std::printf("00c021f0_TEST FAIL\n"); return 1; }
    std::printf("00c021f0_TEST PASS\n");
    return 0;
}
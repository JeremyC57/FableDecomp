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
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))
static void* g_f[4]; static int g_n;
static void CDECL F(voidpf, voidpf p) { g_f[g_n++] = p; }
static int CDECL z_inflateEnd(z_streamp strm)
{
    struct inflate_state* state;
    if (strm == 0 || strm->state == 0 || strm->zfree == (free_func)0) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (state->window != 0) ZFREE(strm, state->window);
    ZFREE(strm, strm->state);
    strm->state = 0;
    return Z_OK;
}
static struct inflate_state g_st;
int main()
{
    int bad = 0; z_stream s; std::memset(&s, 0, sizeof(s));
    unsigned char w[4];
    if (z_inflateEnd(0) != Z_STREAM_ERROR) bad = 1;
    if (z_inflateEnd(&s) != Z_STREAM_ERROR) bad = 2;
    s.state = (struct internal_state*)&g_st;
    if (z_inflateEnd(&s) != Z_STREAM_ERROR) bad = 3;   // no zfree
    s.zfree = F; g_st.window = w;
    if (z_inflateEnd(&s) != Z_OK || g_n != 2 || g_f[0] != w || g_f[1] != &g_st || s.state) bad = 4;
    s.state = (struct internal_state*)&g_st; g_st.window = 0; g_n = 0;
    if (z_inflateEnd(&s) != Z_OK || g_n != 1 || g_f[0] != &g_st) bad = 5;
    if (bad) { std::printf("00c038d0_TEST FAIL\n"); return 1; }
    std::printf("00c038d0_TEST PASS\n");
    return 0;
}
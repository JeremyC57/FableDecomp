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
#define Z_MEM_ERROR (-4)
#define Z_VERSION_ERROR (-6)
#define ZLIB_VERSION "1.2.1"
#define ZALLOC(strm, items, size) (*((strm)->zalloc))((strm)->opaque, (items), (size))
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))
static struct inflate_state g_st; static int g_allocs, g_frees, g_resets; static int g_failAlloc;
static voidpf CDECL zcalloc(voidpf, unsigned items, unsigned size) { g_allocs++; return (items == 1 && size == sizeof(struct inflate_state) && !g_failAlloc) ? (voidpf)&g_st : 0; }
static void CDECL zcfree(voidpf, voidpf p) { if (p == &g_st) g_frees++; }
static int CDECL z_inflateReset(z_streamp strm) { g_resets++; return strm->state ? 0 : -2; }
static int CDECL z_inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size)
{
    struct inflate_state* state;
    if (version == 0 || version[0] != ZLIB_VERSION[0] || stream_size != (int)(sizeof(z_stream))) return Z_VERSION_ERROR;
    if (strm == 0) return Z_STREAM_ERROR;
    strm->msg = 0;
    if (strm->zalloc == (alloc_func)0) { strm->zalloc = zcalloc; strm->opaque = (voidpf)0; }
    if (strm->zfree == (free_func)0) strm->zfree = zcfree;
    state = (struct inflate_state*)ZALLOC(strm, 1, sizeof(struct inflate_state));
    if (state == 0) return Z_MEM_ERROR;
    strm->state = (struct internal_state*)state;
    if (windowBits < 0) { state->wrap = 0; windowBits = -windowBits; }
    else { state->wrap = (windowBits >> 4) + 1; if (windowBits < 48) windowBits &= 15; }
    if (windowBits < 8 || windowBits > 15) { ZFREE(strm, state); strm->state = 0; return Z_STREAM_ERROR; }
    state->wbits = (unsigned)windowBits;
    state->window = 0;
    return z_inflateReset(strm);
}
int main()
{
    int bad = 0; z_stream s;
    if (z_inflateInit2_(&s, 15, 0, sizeof(z_stream)) != Z_VERSION_ERROR) bad = 1;
    if (z_inflateInit2_(&s, 15, "2.0", sizeof(z_stream)) != Z_VERSION_ERROR) bad = 2;
    if (z_inflateInit2_(&s, 15, "1.2.1", 12) != Z_VERSION_ERROR) bad = 3;
    if (z_inflateInit2_(0, 15, "1.2.1", sizeof(z_stream)) != Z_STREAM_ERROR) bad = 4;
    std::memset(&s, 0, sizeof(s)); s.msg = (char*)"m";
    if (z_inflateInit2_(&s, 15, "1.2.1", sizeof(z_stream)) != Z_OK || s.msg || s.zalloc != zcalloc || s.zfree != zcfree ||
        s.state != (struct internal_state*)&g_st || g_st.wrap != 1 || g_st.wbits != 15 || g_st.window || g_resets != 1) bad = 5;
    std::memset(&s, 0, sizeof(s));
    if (z_inflateInit2_(&s, -9, "1.2.1", sizeof(z_stream)) != Z_OK || g_st.wrap != 0 || g_st.wbits != 9) bad = 6;
    std::memset(&s, 0, sizeof(s));
    if (z_inflateInit2_(&s, 31, "1.2.1", sizeof(z_stream)) != Z_OK || g_st.wrap != 2 || g_st.wbits != 15) bad = 7;
    std::memset(&s, 0, sizeof(s));
    if (z_inflateInit2_(&s, 47, "1.2.1", sizeof(z_stream)) != Z_OK || g_st.wrap != 3 || g_st.wbits != 15) bad = 8;
    std::memset(&s, 0, sizeof(s)); g_frees = 0;
    if (z_inflateInit2_(&s, 7, "1.2.1", sizeof(z_stream)) != Z_STREAM_ERROR || s.state || g_frees != 1) bad = 9;
    std::memset(&s, 0, sizeof(s)); g_frees = 0;
    if (z_inflateInit2_(&s, 48, "1.2.1", sizeof(z_stream)) != Z_STREAM_ERROR || g_frees != 1) bad = 10;
    std::memset(&s, 0, sizeof(s)); g_failAlloc = 1;
    if (z_inflateInit2_(&s, 15, "1.2.1", sizeof(z_stream)) != Z_MEM_ERROR) bad = 11;
    if (bad) { std::printf("00c02240_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c02240_TEST PASS\n");
    return 0;
}
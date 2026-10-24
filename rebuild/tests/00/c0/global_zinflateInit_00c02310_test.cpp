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
#define DEF_WBITS 15
static z_streamp g_s; static int g_wb, g_sz; static const char* g_v;
static int CDECL z_inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size)
{ g_s = strm; g_wb = windowBits; g_v = version; g_sz = stream_size; return 5; }
static int CDECL z_inflateInit_(z_streamp strm, const char* version, int stream_size)
{
    return z_inflateInit2_(strm, DEF_WBITS, version, stream_size);
}
int main()
{
    z_stream s; const char* v = "1.2.1";
    if (z_inflateInit_(&s, v, 0x38) != 5 || g_s != &s || g_wb != 15 || g_v != v || g_sz != 0x38)
    { std::printf("00c02310_TEST FAIL\n"); return 1; }
    std::printf("00c02310_TEST PASS\n");
    return 0;
}
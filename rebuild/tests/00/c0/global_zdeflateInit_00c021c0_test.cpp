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
#define Z_DEFLATED 8
#define MAX_WBITS 15
#define DEF_MEM_LEVEL 8
#define Z_DEFAULT_STRATEGY 0
static int g_args[8]; static const char* g_ver; static z_streamp g_strm;
static int CDECL z_deflateInit2_(z_streamp strm, int level, int method, int windowBits,
                                 int memLevel, int strategy, const char* version, int stream_size)
{
    g_strm = strm; g_args[0] = level; g_args[1] = method; g_args[2] = windowBits; g_args[3] = memLevel;
    g_args[4] = strategy; g_ver = version; g_args[5] = stream_size; return -77;
}
static int CDECL z_deflateInit_(z_streamp strm, int level, const char* version, int stream_size)
{
    return z_deflateInit2_(strm, level, Z_DEFLATED, MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, version, stream_size);
}
int main()
{
    z_stream s; const char* v = "1.2.1";
    int r = z_deflateInit_(&s, 9, v, 0x38);
    if (r != -77 || g_strm != &s || g_args[0] != 9 || g_args[1] != 8 || g_args[2] != 15 || g_args[3] != 8 ||
        g_args[4] != 0 || g_ver != v || g_args[5] != 0x38) { std::printf("00c021c0_TEST FAIL\n"); return 1; }
    std::printf("00c021c0_TEST PASS\n");
    return 0;
}
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
typedef uLong uLongf;
#define Z_OK 0
#define Z_STREAM_END 1
#define Z_FINISH 4
#define Z_BUF_ERROR (-5)
#define ZLIB_VERSION "1.2.1"
static int g_initRet, g_deflRet, g_endRet, g_ends, g_lvl; static uLong g_total;
static int CDECL z_deflateInit_(z_streamp strm, int level, const char* version, int stream_size)
{ g_lvl = level; if (version[0] != '1' || stream_size != (int)sizeof(z_stream) || strm->zalloc || strm->zfree || strm->opaque) return -99; return g_initRet; }
static int CDECL z_deflate(z_streamp strm, int flush) { if (flush != Z_FINISH) return -98; strm->total_out = g_total; return g_deflRet; }
static int CDECL z_deflateEnd(z_streamp) { g_ends++; return g_endRet; }
#define deflateInit(strm, level) z_deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream))
static int CDECL z_compress2(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen, int level)
{
    z_stream stream; int err;
    stream.next_in = (Bytef*)source; stream.avail_in = (uInt)sourceLen;
    stream.next_out = dest; stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;
    stream.zalloc = (alloc_func)0; stream.zfree = (free_func)0; stream.opaque = (voidpf)0;
    err = deflateInit(&stream, level);
    if (err != Z_OK) return err;
    err = z_deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) { z_deflateEnd(&stream); return err == Z_OK ? Z_BUF_ERROR : err; }
    *destLen = stream.total_out;
    err = z_deflateEnd(&stream);
    return err;
}
int main()
{
    int bad = 0; Bytef d[8], src[8]; uLongf len = 8;
    g_initRet = -2; if (z_compress2(d, &len, src, 8, 6) != -2 || g_lvl != 6 || g_ends) bad = 1;
    g_initRet = 0; g_deflRet = 0; g_ends = 0; if (z_compress2(d, &len, src, 8, 6) != Z_BUF_ERROR || g_ends != 1 || len != 8) bad = 2;
    g_deflRet = -3; g_ends = 0; if (z_compress2(d, &len, src, 8, 6) != -3 || g_ends != 1) bad = 3;
    g_deflRet = Z_STREAM_END; g_total = 5; g_endRet = 0; g_ends = 0;
    if (z_compress2(d, &len, src, 8, 9) != Z_OK || len != 5 || g_ends != 1) bad = 4;
    if (bad) { std::printf("00c03de0_TEST FAIL\n"); return 1; }
    std::printf("00c03de0_TEST PASS\n");
    return 0;
}
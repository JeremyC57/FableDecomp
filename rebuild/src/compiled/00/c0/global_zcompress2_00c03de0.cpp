// _global::z_compress2(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen, int level) -- zlib 1.2.1 compress2: one-shot deflate of source into dest through a stack z_stream (deflateInit / deflate(Z_FINISH) / deflateEnd)
// Leaf: _z_compress2
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C03DE0, 169 bytes. zlib 1.2.1 compress.c, Z_PREFIX build, C linkage, __cdecl.
// Callees: z_deflateInit_ 0x00C021C0 (the deflateInit macro: level, ZLIB_VERSION 0x0129CFE0, sizeof(z_stream) 0x38),
// z_deflate 0x00C006A0 (Z_FINISH = 4), z_deflateEnd 0x00C00AE0.
// Shape: the 0x38-byte z_stream is the whole frame (`sub esp,38h`); MAXSEG_64K is not defined, so only the
// avail_out round-trip check survives and folds away (uInt == uLong on Win32); destLen stays in ebx for the final
// `*destLen = stream.total_out` store; `err == Z_OK ? Z_BUF_ERROR : err` is the `mov eax,-5 ; je` select.
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
typedef uLong uLongf;

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_FINISH        4
#define Z_BUF_ERROR    (-5)
#define ZLIB_VERSION "1.2.1"

extern "C" int __cdecl z_deflateInit_(z_streamp strm, int level, const char* version, int stream_size);   // 0x00C021C0
extern "C" int __cdecl z_deflate(z_streamp strm, int flush);                                             // 0x00C006A0
extern "C" int __cdecl z_deflateEnd(z_streamp strm);                                                     // 0x00C00AE0
#define deflateInit(strm, level) z_deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream))

extern "C" int __cdecl z_compress2(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen, int level)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit(&stream, level);
    if (err != Z_OK) return err;

    err = z_deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        z_deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = z_deflateEnd(&stream);
    return err;
}
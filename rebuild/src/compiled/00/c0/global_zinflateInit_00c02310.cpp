// _global::z_inflateInit_(z_streamp strm, const char* version, int stream_size) -- zlib 1.2.1 inflateInit_: forward to inflateInit2_ with DEF_WBITS (15)
// Leaf: _z_inflateInit_
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C02310, 26 bytes. zlib 1.2.1, Z_PREFIX build, C linkage, __cdecl.
// The catalog label "ConstructString" is propagated/wrong: the body forwards (strm, 15, version, stream_size) to
// z_inflateInit2_ 0x00C02240 (landed sibling in this chunk), and the Ego_r/FableWin PDB name sets list
// _z_inflateInit_ directly after _z_inflateInit2_ in inflate.c order.
// Shape: three argument loads (eax/ecx/edx) before the pushes, DEF_WBITS pushed as an immediate, `add esp,10h`.
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

#define DEF_WBITS 15   /* MAX_WBITS */

extern "C" int __cdecl z_inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size);   // 0x00C02240

extern "C" int __cdecl z_inflateInit_(z_streamp strm, const char* version, int stream_size)
{
    return z_inflateInit2_(strm, DEF_WBITS, version, stream_size);
}
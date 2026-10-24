// _global::z_deflateInit_(z_streamp strm, int level, const char* version, int stream_size) -- zlib 1.2.1 deflateInit_: forward to deflateInit2_ with Z_DEFLATED, MAX_WBITS, DEF_MEM_LEVEL and Z_DEFAULT_STRATEGY
// Leaf: _z_deflateInit_
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C021C0, 37 bytes. zlib 1.2.1, Z_PREFIX build, C linkage, __cdecl.
// Callee: z_deflateInit2_ 0x00C01F90 (__cdecl, 8 arguments: strm, level, method 8, windowBits 15, memLevel 8,
// strategy 0, version, stream_size). Shape: VC pushes right to left, loading the two pass-through stack arguments
// into eax/ecx first and edx/eax for the last two, then `add esp,20h` after the call.
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

#define Z_DEFLATED           8
#define MAX_WBITS           15
#define DEF_MEM_LEVEL        8
#define Z_DEFAULT_STRATEGY   0

extern "C" int __cdecl z_deflateInit2_(z_streamp strm, int level, int method, int windowBits,
                                      int memLevel, int strategy, const char* version, int stream_size);   // 0x00C01F90

extern "C" int __cdecl z_deflateInit_(z_streamp strm, int level, const char* version, int stream_size)
{
    return z_deflateInit2_(strm, level, Z_DEFLATED, MAX_WBITS, DEF_MEM_LEVEL,
                           Z_DEFAULT_STRATEGY, version, stream_size);
    /* To do: ignore strm->next_in if we use it as window */
}
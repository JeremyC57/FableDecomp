// CCompressorZlib::CompressStream(void* dest, unsigned long destSize) -- lazily deflateInit the stream at level 9, point the output at the caller's buffer, run deflate() with no flush and return the bytes produced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3A7F0, length 80 bytes.
// Callees: deflateInit_ @ 0x00C021C0 (the deflateInit(strm, level) macro: version string "1.1.4" at
// 0x0129CFE0, sizeof(z_stream) = 56), deflate @ 0x00C006A0 (flush = Z_NO_FLUSH); both __cdecl with the
// results dropped (`add esp,N`). Retail layout (engine/CCompressorZlib.h minus 4: the base's vector is
// 12 bytes in the shipped binary): z_stream at +0x1c (next_out +0x28, avail_out +0x2c), ZlibInitialised
// +0x54, Compressing +0x55. The two flags are set from one `mov al,1`; the bytes produced are
// destSize - avail_out, with destSize kept in edi across the call.
#include "engine/CCompressorZlib.h"

struct z_stream_s {                                             // zlib 1.1.4 stream (56 bytes)
    unsigned char* next_in;  unsigned int avail_in;  unsigned long total_in;
    unsigned char* next_out; unsigned int avail_out; unsigned long total_out;
    char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int);
    void  (__cdecl* zfree)(void*, void*);
    void* opaque; int data_type; unsigned long adler; unsigned long reserved;
};

extern "C" int __cdecl deflateInit_(z_stream_s* strm, int level, const char* version, int stream_size);   // 0x00C021C0
extern "C" int __cdecl deflate(z_stream_s* strm, int flush);                                            // 0x00C006A0
#define ZLIB_VERSION "1.1.4"
#define deflateInit(strm, level) deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream_s))

struct CCompressorZlib_Retail {
    virtual void v00();
    unsigned char _base[0x18];                                  // +0x04..+0x1b CACompressor (workspace, flag, 12-byte vector)
    z_stream_s    Stream;                                       // +0x1c
    bool          ZlibInitialised;                              // +0x54
    bool          Compressing;                                  // +0x55
    bool          ErrorFlag;                                    // +0x56
    unsigned long CompressStream(void* dest, unsigned long destSize);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == 0x58);

unsigned long CCompressorZlib_Retail::CompressStream(void* dest, unsigned long destSize)
{
    if (!ZlibInitialised) {
        deflateInit(&Stream, 9);
        ZlibInitialised = true;
        Compressing = true;
    }
    Stream.next_out = (unsigned char*)dest;
    Stream.avail_out = destSize;
    deflate(&Stream, 0);
    return destSize - Stream.avail_out;
}

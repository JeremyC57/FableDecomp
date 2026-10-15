// CCompressorZlib::DecompressStream(void* dest, unsigned long destSize, bool* endOfStream) -- lazily inflateInit the stream, inflate into the caller's buffer with Z_SYNC_FLUSH, flag errors and end of stream, and return the bytes produced
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3A840, 117 bytes (128 with padding).
// Callees: inflateInit_ 0x00C02310 (version "1.1.4" at 0x0129CFE0, sizeof(z_stream) = 0x38), inflate 0x00C02470.
// Retail layout (as the landed CompressStream 0x00A3A7F0): engine/CCompressorZlib.h minus 4 bytes (the
// base's vector is 12 bytes in the shipped binary): z_stream at +0x1c (next_out +0x28, avail_out +0x2c),
// ZlibInitialised +0x54, Compressing +0x55, ErrorFlag +0x56.
// Shape: the decompressing twin of CompressStream: the flags are set as true/false immediates, the
// stream end (Z_STREAM_END) stores true and returns at once, any other non-Z_OK result raises ErrorFlag;
// both exits recompute destSize - avail_out (destSize kept in edi).
#include "engine/CCompressorZlib.h"

struct z_stream_s {                                             // zlib 1.1.4 stream (56 bytes)
    unsigned char* next_in;  unsigned int avail_in;  unsigned long total_in;
    unsigned char* next_out; unsigned int avail_out; unsigned long total_out;
    char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int);
    void  (__cdecl* zfree)(void*, void*);
    void* opaque; int data_type; unsigned long adler; unsigned long reserved;
};

#define ZLIB_VERSION "1.1.4"
#define Z_OK 0
#define Z_STREAM_END 1
#define Z_SYNC_FLUSH 2
#define Z_FINISH 4
extern "C" int __cdecl inflateInit_(z_stream_s* strm, const char* version, int stream_size);   // 0x00C02310
extern "C" int __cdecl inflate(z_stream_s* strm, int flush);                                       // 0x00C02470
#define inflateInit(strm) inflateInit_((strm), ZLIB_VERSION, sizeof(z_stream_s))

struct CCompressorZlib_Retail {
    virtual void v00();
    unsigned char _base[0x18];                                  // +0x04..+0x1b CACompressor (workspace, flag, 12-byte vector)
    z_stream_s    Stream;                                       // +0x1c
    bool          ZlibInitialised;                              // +0x54
    bool          Compressing;                                  // +0x55
    bool          ErrorFlag;                                    // +0x56
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == sizeof(CCompressorZlib) - 4);

unsigned long CCompressorZlib_Retail::DecompressStream(void* dest, unsigned long destSize, bool* endOfStream)
{
    if (!ZlibInitialised) {
        inflateInit(&Stream);
        ZlibInitialised = true;
        Compressing = false;
    }
    Stream.next_out = (unsigned char*)dest;
    Stream.avail_out = destSize;
    int result = inflate(&Stream, Z_SYNC_FLUSH);
    if (result == Z_STREAM_END) {
        *endOfStream = true;
        return destSize - Stream.avail_out;
    }
    if (result != Z_OK)
        ErrorFlag = true;
    *endOfStream = false;
    return destSize - Stream.avail_out;
}

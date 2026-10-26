// CCompressorZlib::FinishCompressStream(void* dest, unsigned long destSize, bool* finished) -- deflate with Z_FINISH into the caller's buffer; at the stream end tear the deflate state down, otherwise flag errors; return the bytes produced
// Leaf: ?FinishCompressStream@CCompressorZlib_Retail@@QAEKPAXKPA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3A8C0, 107 bytes (112 with padding).
// Callees: z_deflate 0x00C006A0 (flush = Z_FINISH), z_deflateEnd 0x00C00AE0.
// Retail layout (as the landed CompressStream 0x00A3A7F0): engine/CCompressorZlib.h minus 4 bytes (the
// base's vector is 12 bytes in the shipped binary): z_stream at +0x1c (next_out +0x28, avail_out +0x2c),
// ZlibInitialised +0x54, Compressing +0x55, ErrorFlag +0x56.
// Shape: `finished` is stored twice on the end path: once right after deflate (VC reuses al == 1 from
// the compared result) and again after the teardown; the teardown clears both flags.
#include "engine/CCompressorZlib.h"

struct z_stream_s {                                             // zlib 1.2.1 stream (56 bytes)
    unsigned char* next_in;  unsigned int avail_in;  unsigned long total_in;
    unsigned char* next_out; unsigned int avail_out; unsigned long total_out;
    char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int);
    void  (__cdecl* zfree)(void*, void*);
    void* opaque; int data_type; unsigned long adler; unsigned long reserved;
};

#define ZLIB_VERSION "1.2.1"
#define Z_OK 0
#define Z_STREAM_END 1
#define Z_SYNC_FLUSH 2
#define Z_FINISH 4
extern "C" int __cdecl z_deflate(z_stream_s* strm, int flush);                                       // 0x00C006A0
extern "C" int __cdecl z_deflateEnd(z_stream_s* strm);                                               // 0x00C00AE0

struct CCompressorZlib_Retail {
    virtual void v00();
    unsigned char _base[0x18];                                  // +0x04..+0x1b CACompressor (workspace, flag, 12-byte vector)
    z_stream_s    Stream;                                       // +0x1c
    bool          ZlibInitialised;                              // +0x54
    bool          Compressing;                                  // +0x55
    bool          ErrorFlag;                                    // +0x56
    unsigned long FinishCompressStream(void* dest, unsigned long destSize, bool* finished);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == sizeof(CCompressorZlib)); // engine header: retail STLport 4.6 layout

unsigned long CCompressorZlib_Retail::FinishCompressStream(void* dest, unsigned long destSize, bool* finished)
{
    Stream.next_out = (unsigned char*)dest;
    Stream.avail_out = destSize;
    int result = z_deflate(&Stream, Z_FINISH);
    if (result == Z_STREAM_END) {
        *finished = true;
        z_deflateEnd(&Stream);
        ZlibInitialised = false;
        Compressing = false;
        *finished = true;
        return destSize - Stream.avail_out;
    }
    if (result != Z_OK)
        ErrorFlag = true;
    *finished = false;
    return destSize - Stream.avail_out;
}
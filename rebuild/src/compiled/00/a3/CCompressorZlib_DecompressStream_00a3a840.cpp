// CCompressorZlib::DecompressStream(void* dest, unsigned long destSize, bool& endOfStream) -- lazily inflateInit the stream, inflate into the caller's buffer with Z_SYNC_FLUSH, flag errors and end of stream, and return the bytes produced
// Leaf: ?DecompressStream@CCompressorZlib@@QAEKPAXKAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3A840, 117 bytes (128 with padding).
// Callees: z_inflateInit_ 0x00C02310 (version "1.2.1" at 0x0129CFE0, sizeof(z_stream) = 0x38), z_inflate 0x00C02470.
// Retail layout (as the landed CompressStream 0x00A3A7F0): engine/CCompressorZlib.h minus 4 bytes (the
// base's vector is 12 bytes in the shipped binary): z_stream at +0x1c (next_out +0x28, avail_out +0x2c),
// ZlibInitialised +0x54, Compressing +0x55, ErrorFlag +0x56.
// Shape: the decompressing twin of CompressStream: the flags are set as true/false immediates, the
// stream end (Z_STREAM_END) stores true and returns at once, any other non-Z_OK result raises ErrorFlag;
// both exits recompute destSize - avail_out (destSize kept in edi).
// NF176 (2026-11-20, q5003): the CCompressorZlib_Retail stand-in and the local z_stream_s are gone: engine/CCompressorZlib.h
//   (FABLE_RETAIL_CCOMPRESSORZLIB: the retail layout, Stream +0x1C, ZlibInitialised / Compressing / ErrorFlag +0x54..+0x56; _STREAM:
//   DecompressStream with the bool reference). The leaf is the Ego_r.pdb 0x00B956A0 / FableWin.pdb 0x031FC180
//   ?DecompressStream@CCompressorZlib@@QAEKPAXKAA_N@Z (the end-of-stream flag is a bool&, as 0x00994A80 / 0x009C3C20 bind it); was
//   ?DecompressStream@CCompressorZlib_Retail@@QAEKPAXKPA_N@Z. Body unchanged.
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM
#include "engine/CCompressorZlib.h"

#define ZLIB_VERSION "1.2.1"
#define Z_OK 0
#define Z_STREAM_END 1
#define Z_SYNC_FLUSH 2
#define Z_FINISH 4
extern "C" int __cdecl z_inflateInit_(z_stream_s* strm, const char* version, int stream_size);   // 0x00C02310
extern "C" int __cdecl z_inflate(z_stream_s* strm, int flush);                                       // 0x00C02470
#define inflateInit(strm) z_inflateInit_((strm), ZLIB_VERSION, sizeof(z_stream_s))

unsigned long CCompressorZlib::DecompressStream(void* dest, unsigned long destSize, bool& endOfStream)
{
    if (!ZlibInitialised) {
        inflateInit(&Stream);
        ZlibInitialised = true;
        Compressing = false;
    }
    Stream.next_out = (unsigned char*)dest;
    Stream.avail_out = destSize;
    int result = z_inflate(&Stream, Z_SYNC_FLUSH);
    if (result == Z_STREAM_END) {
        endOfStream = true;
        return destSize - Stream.avail_out;
    }
    if (result != Z_OK)
        ErrorFlag = true;
    endOfStream = false;
    return destSize - Stream.avail_out;
}
// CCompressorZlib::InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize) -- one-shot inflate of a whole block through the shared static stream (initialised once with the engine's allocators, reset afterwards); returns total_out
// Leaf: ?InternalDecompress@CCompressorZlib_Retail@@QAEKPBXKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3AB00, 139 bytes (144 with padding).
// Callees: z_inflateInit_ 0x00C02310 (version "1.2.1" at 0x0129CFE0, sizeof(z_stream) = 0x38),
//          z_inflateReset 0x00C021F0, z_inflate 0x00C02470 (flush = Z_FINISH).
// Globals: the static decompression state at 0x013D27EC: a z_stream (total_out 0x013D2800, zalloc
//          0x013D280C, zfree 0x013D2810) followed by its "initialised" byte at +0x38 (0x013D2824);
//          the allocator hooks 0x00A3A9C0 (overflow-checked malloc) and 0x00A3AA00.
//
// Shape: `this` is never read (ret 10h on a thiscall member, as the landed Decompress 0x00A3ABE0
// calls it). An empty source returns 0 straight from the tested register. The four stream fields are
// written before the first-use test; the first call installs the allocators and inflateInit()s, later
// calls inflateReset(); the single inflate(Z_FINISH) result is ignored and total_out returned.
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
extern "C" int __cdecl z_inflateInit_(z_stream_s* strm, const char* version, int stream_size);   // 0x00C02310
extern "C" int __cdecl z_inflateReset(z_stream_s* strm);                                             // 0x00C021F0
extern "C" int __cdecl z_inflate(z_stream_s* strm, int flush);                                       // 0x00C02470
#define inflateInit(strm) z_inflateInit_((strm), ZLIB_VERSION, sizeof(z_stream_s))

void* __cdecl ZlibAlloc(void* opaque, unsigned int items, unsigned int size);   // 0x00A3A9C0
void __cdecl ZlibFree(void* opaque, void* address);                             // 0x00A3AA00

struct SStaticInflateState {
    z_stream_s Stream;                                          // 0x013D27EC
    bool       Initialised;                                     // 0x013D2824
};
extern SStaticInflateState g_staticInflate;                     // 0x013D27EC

struct CCompressorZlib_Retail {
    virtual void v00();
    unsigned char _body[0x54];                                  // +0x04..+0x57 (CACompressor, z_stream, flags)
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);
};

FABLE_STATIC_ASSERT(sizeof(CCompressorZlib_Retail) == sizeof(CCompressorZlib)); // engine header: retail STLport 4.6 layout

unsigned long CCompressorZlib_Retail::InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize)
{
    if (sourceSize == 0)
        return 0;
    g_staticInflate.Stream.next_in = (unsigned char*)source;
    g_staticInflate.Stream.avail_in = sourceSize;
    g_staticInflate.Stream.next_out = (unsigned char*)dest;
    g_staticInflate.Stream.avail_out = destSize;
    if (!g_staticInflate.Initialised) {
        g_staticInflate.Stream.zalloc = ZlibAlloc;
        g_staticInflate.Stream.zfree = ZlibFree;
        inflateInit(&g_staticInflate.Stream);
        g_staticInflate.Initialised = true;
    } else {
        z_inflateReset(&g_staticInflate.Stream);
    }
    z_inflate(&g_staticInflate.Stream, Z_FINISH);
    return g_staticInflate.Stream.total_out;
}
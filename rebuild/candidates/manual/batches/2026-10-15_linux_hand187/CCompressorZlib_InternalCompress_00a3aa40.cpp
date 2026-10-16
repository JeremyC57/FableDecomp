// CCompressorZlib::InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize) -- deflate a block at level 1 through one-byte in/out windows (zlib example.c style) and return the compressed size
// Leaf: InternalCompress
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3AA40, 189 bytes (192 with padding), `ret 0x10`; `this` is not used.
// Callees: zlib deflateInit_ 0x00C021C0 (version string 0x0129CFE0, sizeof(z_stream) 0x38), deflate 0x00C006A0,
//   deflateEnd 0x00C00AE0; allocator callbacks CCompressorZlib zalloc 0x00A3A9C0 / zfree 0x00A3AA00.
// Layout: the z_stream is a 0x38-byte local (next_in +0, avail_in +4, total_in +8, next_out +0xc, avail_out +0x10,
//   total_out +0x14, ..., zalloc +0x20, zfree +0x24, opaque +0x28), the same record the engine header embeds at +0x20.
// Shape: deflateInit is the zlib macro (level, ZLIB_VERSION, sizeof); the pump loop is
//   `while (total_in != size && total_out < destSize)` with both windows set to 1 byte, then Z_FINISH until Z_STREAM_END.
typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef void* (__cdecl *alloc_func)(void* opaque, uInt items, uInt size);
typedef void  (__cdecl *free_func)(void* opaque, void* address);
struct internal_state;

struct z_stream {
    Bytef*          next_in;      // +0x00
    uInt            avail_in;     // +0x04
    uLong           total_in;     // +0x08
    Bytef*          next_out;     // +0x0c
    uInt            avail_out;    // +0x10
    uLong           total_out;    // +0x14
    char*           msg;          // +0x18
    internal_state* state;        // +0x1c
    alloc_func      zalloc;       // +0x20
    free_func       zfree;        // +0x24
    void*           opaque;       // +0x28
    int             data_type;    // +0x2c
    uLong           adler;        // +0x30
    uLong           reserved;     // +0x34
};

extern "C" int __cdecl deflateInit_(z_stream* strm, int level, const char* version, int stream_size);   // 0x00C021C0
extern "C" int __cdecl deflate(z_stream* strm, int flush);                                             // 0x00C006A0
extern "C" int __cdecl deflateEnd(z_stream* strm);                                                      // 0x00C00AE0
extern const char g_zlibVersion[];                                                                      // 0x0129CFE0 ("1.x.x")

struct CCompressorZlib {
    static void* __cdecl ZAlloc(void* opaque, uInt items, uInt size);   // 0x00A3A9C0
    static void  __cdecl ZFree(void* opaque, void* address);            // 0x00A3AA00
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);
};

unsigned long CCompressorZlib::InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize)
{
    if (sourceSize == 0)
        return 0;
    z_stream stream;
    stream.zalloc = ZAlloc;
    stream.zfree = ZFree;
    stream.opaque = 0;
    deflateInit_(&stream, 1, g_zlibVersion, sizeof(z_stream));
    stream.next_in = (Bytef*)source;
    stream.next_out = (Bytef*)dest;
    while (stream.total_in != sourceSize && stream.total_out < destSize) {
        stream.avail_in = stream.avail_out = 1;
        deflate(&stream, 0);
    }
    for (;;) {
        stream.avail_out = 1;
        if (deflate(&stream, 4) == 1)
            break;
    }
    deflateEnd(&stream);
    return stream.total_out;
}

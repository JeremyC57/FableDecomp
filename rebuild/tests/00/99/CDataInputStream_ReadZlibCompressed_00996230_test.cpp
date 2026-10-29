#include <cstdio>
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStream_Methods   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern void (__cdecl* g_pfnAssertionFailed)();               // 0x013BC9FC: installed debug hook

class CACompressor { public:                                        // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);                        // 0x00A3AEA0
};
class CCompressorZlib : public CACompressor { public:                      // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                          // 0x00A3A7D0
    void Init();                                             // 0x00A3AA10
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
    virtual ~CCompressorZlib();                              // 0x00A3ACC0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};
struct CByteArray {                                          // retail vector<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                             // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
};

struct CDataInputStream_Methods : CDataInputStream {
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    void ReadZlibCompressed(void* data, long size);
};

void CDataInputStream_Methods::ReadZlibCompressed(void* data, long size)
{
    if (size > 0) {
        long compressedSize = ReadLong();
        CByteArray buffer(compressedSize);
        Read(buffer.GetData(), compressedSize);
        CCompressorZlib compressor;
        unsigned long written = compressor.InternalDecompress(buffer.GetData(), compressedSize, data, size);
        if (written != (unsigned long)size) {
            if (g_pfnAssertionFailed != 0)
                g_pfnAssertionFailed();
        }
    }
}

#include <string.h>
#include <stdlib.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
static int g_overflows = 0, g_asserts = 0, g_inits = 0, g_dtors = 0, g_frees = 0;
static unsigned char g_overflowData[64];
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflows; memcpy(buffer, g_overflowData, size); StreamPos += size;
}
static void __cdecl AssertHook() { ++g_asserts; }
void (__cdecl* g_pfnAssertionFailed)() = 0;
CACompressor::CACompressor(long workspaceSize) {}
CACompressor::~CACompressor() {}
long CCompressorZlib::GetWorkspaceSize() { return 16; }
void CCompressorZlib::Init() { ++g_inits; }
CCompressorZlib::~CCompressorZlib() { ++g_dtors; }
static unsigned char g_lastSource[64]; static unsigned long g_lastSourceSize, g_lastDestSize; static void* g_lastDest; static unsigned long g_decompressResult;
unsigned long CCompressorZlib::InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize)
{
    memcpy(g_lastSource, source, sourceSize < 64 ? sourceSize : 64); g_lastSourceSize = sourceSize; g_lastDest = dest; g_lastDestSize = destSize;
    return g_decompressResult;
}
CByteArray::CByteArray(unsigned long n)
{
    m_begin = n ? (unsigned char*)calloc(n, 1) : 0; m_end = m_begin + n; m_capacity = m_end;
}
extern "C" void __cdecl free(void* block);
int main()
{
    static unsigned char chunk[64];
    static CDataInputStream_Methods s;
    long cs = 5; memcpy(chunk, &cs, 4); memcpy(chunk + 4, "ABCDE", 5);
    char out[32];
    // size <= 0: nothing read
    s.StreamPos = 0; s.CurrentSourcePtr = chunk; s.SourceChunkBytesRemaining = 9;
    s.ReadZlibCompressed(out, 0);
    check(s.StreamPos == 0 && g_inits == 0, "size 0 no-op");
    // fast path: count and data from the current chunk
    g_decompressResult = 20;
    s.ReadZlibCompressed(out, 20);
    check(s.StreamPos == 9 && s.CurrentSourcePtr == chunk + 9 && s.SourceChunkBytesRemaining == 0, "chunk consumed");
    check(g_lastSourceSize == 5 && memcmp(g_lastSource, "ABCDE", 5) == 0 && g_lastDest == out && g_lastDestSize == 20, "decompress args");
    check(g_inits == 1 && g_dtors == 1 && g_asserts == 0 && g_overflows == 0, "compressor lifetime, no assert");
    // short output fires the installed hook; uninstalled hook is skipped
    g_pfnAssertionFailed = AssertHook; s.StreamPos = 0; s.CurrentSourcePtr = chunk; s.SourceChunkBytesRemaining = 9; g_decompressResult = 7;
    s.ReadZlibCompressed(out, 20);
    check(g_asserts == 1, "assert hook on short output");
    // overflow path: count read through the overflow reader, data too
    long cs2 = 3; memcpy(g_overflowData, &cs2, 4);
    s.StreamPos = 0; s.CurrentSourcePtr = chunk; s.SourceChunkBytesRemaining = 2; g_decompressResult = 20;
    s.ReadZlibCompressed(out, 20);
    check(g_overflows == 2 && g_lastSourceSize == 3, "overflow reads");
    // zero compressed size: empty buffer (GetData is the array's own address), no data read
    long zero = 0; memcpy(chunk, &zero, 4);
    s.StreamPos = 0; s.CurrentSourcePtr = chunk; s.SourceChunkBytesRemaining = 9;
    s.ReadZlibCompressed(out, 20);
    check(s.StreamPos == 4 && g_lastSourceSize == 0, "empty block");
    // out-of-range position: nothing read (the count stays uninitialised, so only the position is checked)
    if (!g_fail) printf("00996230_TEST PASS\n");
    return g_fail;
}
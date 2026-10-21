#include <cstdio>
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream {
public:
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual void _slot6();
    virtual void _slot7();
public:
    virtual void InternalReadNextSourceChunk(const unsigned char*& sourcePtr, long& sourceBytes);   // +0x20
    virtual bool NeedsBufferedRead(long size);                                                     // +0x24
    virtual void InternalReadUnbuffered(unsigned char* buffer, long size);                         // +0x28
public:
    unsigned long StreamPos;                                 // +0x04
    long StreamSize;                                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long SourceChunkStreamPos;                      // +0x10
    long SourceChunkBytesRemaining;                          // +0x14

    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
public:
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
public:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);
};

void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    long remaining = SourceChunkBytesRemaining;
    if (remaining > 0) {
        unsigned char* next = buffer + remaining;
        long left = size - remaining;
        if (IsValidPosition((__int64)(StreamPos + remaining))) {
            memcpy(buffer, CurrentSourcePtr, remaining);
            CurrentSourcePtr += remaining;
            SourceChunkBytesRemaining -= remaining;
            StreamPos += remaining;
        }
        buffer = next;
        size = left;
    }
    if (NeedsBufferedRead(size)) {
        while (size > 0) {
            SourceChunkStreamPos = StreamPos;
            InternalReadNextSourceChunk(CurrentSourcePtr, SourceChunkBytesRemaining);
            long chunk = size < SourceChunkBytesRemaining ? size : SourceChunkBytesRemaining;
            Read(buffer, chunk);
            buffer += chunk;
            size -= chunk;
        }
    }
    else {
        unsigned long position = StreamPos;
        InternalReadUnbuffered(buffer, size);
        StreamPos = position + size;
        SourceChunkBytesRemaining = 0;
        CurrentSourcePtr = 0;
        SourceChunkStreamPos = 0;
    }
}

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
void CDataInputStream::_slot0() {}
void CDataInputStream::_slot1() {}
void CDataInputStream::_slot2() {}
void CDataInputStream::_slot3() {}
void CDataInputStream::_slot4() {}
void CDataInputStream::_slot5() {}
void CDataInputStream::_slot6() {}
void CDataInputStream::_slot7() {}
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&, long&) {}
bool CDataInputStream::NeedsBufferedRead(long) { return false; }
void CDataInputStream::InternalReadUnbuffered(unsigned char*, long) {}

static const unsigned char* g_chunks[4]; static long g_chunkSizes[4]; static int g_nextChunk;
static bool g_buffered; static long g_needsArg; static int g_needsCalls;
static unsigned char* g_unbufBuf; static long g_unbufSize; static int g_unbufCalls; static unsigned long g_chunkStreamPosSeen[4];
struct TestStream : CDataInputStream {
    void InternalReadNextSourceChunk(const unsigned char*& sourcePtr, long& sourceBytes)
    {
        g_chunkStreamPosSeen[g_nextChunk] = SourceChunkStreamPos;
        sourcePtr = g_chunks[g_nextChunk]; sourceBytes = g_chunkSizes[g_nextChunk]; ++g_nextChunk;
    }
    bool NeedsBufferedRead(long size) { g_needsArg = size; ++g_needsCalls; return g_buffered; }
    void InternalReadUnbuffered(unsigned char* buffer, long size)
    {
        ++g_unbufCalls; g_unbufBuf = buffer; g_unbufSize = size; memset(buffer, 'U', size);
    }
    void Reset(unsigned long pos, const unsigned char* p, long rem)
    {
        StreamPos = pos; CurrentSourcePtr = p; SourceChunkBytesRemaining = rem; SourceChunkStreamPos = 77;
        g_nextChunk = 0; g_needsCalls = 0; g_unbufCalls = 0; g_needsArg = -1; g_unbufBuf = 0; g_unbufSize = -1;
    }
};
int main()
{
    static TestStream s;
    static const unsigned char c0[] = "abc", c1[] = "defg", c2[] = "hijkl";
    unsigned char out[32];
    // buffered: drain 3, then chunk of 4, then 1 byte of a 5-byte chunk
    g_chunks[0] = c1; g_chunkSizes[0] = 4; g_chunks[1] = c2; g_chunkSizes[1] = 5; g_buffered = true;
    memset(out, 0, sizeof out); s.Reset(100, c0, 3);
    s.ReadWithSrcChunkOverflow(out, 8);
    check(memcmp(out, "abcdefgh", 8) == 0 && out[8] == 0, "buffered data");
    check(g_needsCalls == 1 && g_needsArg == 5, "NeedsBufferedRead(size after drain)");
    check(g_nextChunk == 2 && g_chunkStreamPosSeen[0] == 103 && g_chunkStreamPosSeen[1] == 107, "chunk stream positions");
    check(s.StreamPos == 108 && s.CurrentSourcePtr == c2 + 1 && s.SourceChunkBytesRemaining == 4 && s.SourceChunkStreamPos == 107, "buffered state");
    check(g_unbufCalls == 0, "no unbuffered read");
    // unbuffered, empty window
    g_buffered = false; memset(out, 0, sizeof out); s.Reset(10, c0, 0);
    s.ReadWithSrcChunkOverflow(out, 5);
    check(g_needsArg == 5 && g_unbufCalls == 1 && g_unbufBuf == out && g_unbufSize == 5 && memcmp(out, "UUUUU", 5) == 0, "unbuffered args");
    check(s.StreamPos == 15 && s.CurrentSourcePtr == 0 && s.SourceChunkBytesRemaining == 0 && s.SourceChunkStreamPos == 0, "window cleared");
    // unbuffered after a 2-byte drain
    memset(out, 0, sizeof out); s.Reset(20, c0, 2);
    s.ReadWithSrcChunkOverflow(out, 6);
    check(memcmp(out, "abUUUU", 6) == 0 && g_unbufBuf == out + 2 && g_unbufSize == 4 && g_needsArg == 4, "drain then unbuffered");
    check(s.StreamPos == 26 && s.CurrentSourcePtr == 0, "drain then unbuffered state");
    // drain out of range: nothing copied, buffer/size still advanced, chunk window untouched until cleared
    memset(out, 0, sizeof out); s.Reset(0x7ffffffeUL, c0, 3);
    s.ReadWithSrcChunkOverflow(out, 5);
    check(out[0] == 0 && out[2] == 0 && g_unbufBuf == out + 3 && g_unbufSize == 2 && memcmp(out + 3, "UU", 2) == 0, "out-of-range drain");
    check(s.StreamPos == 0x80000000UL, "out-of-range drain position");
    // buffered with nothing left to read after the drain: loop does not run
    g_buffered = true; memset(out, 0, sizeof out); s.Reset(5, c0, 3);
    s.ReadWithSrcChunkOverflow(out, 3);
    check(memcmp(out, "abc", 3) == 0 && g_nextChunk == 0 && s.StreamPos == 8 && s.SourceChunkBytesRemaining == 0 && s.SourceChunkStreamPos == 77, "buffered exact drain");
    // buffered, loop chunk out of range: chunk fetched, nothing copied, request still counted down
    g_chunks[0] = c1; g_chunkSizes[0] = 4; memset(out, 0, sizeof out); s.Reset(0x7fffffffUL, c0, 0);
    s.ReadWithSrcChunkOverflow(out, 2);
    check(g_nextChunk == 1 && out[0] == 0 && s.SourceChunkBytesRemaining == 4 && s.CurrentSourcePtr == c1 && s.StreamPos == 0x7fffffffUL, "loop out of range");
    if (!g_fail) printf("00993ca0_TEST PASS\n");
    return g_fail;
}
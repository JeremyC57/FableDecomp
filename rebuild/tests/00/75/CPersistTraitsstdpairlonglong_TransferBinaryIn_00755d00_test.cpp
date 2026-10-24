// Behaviour test for 0x00755D00 CPersistTraits<std::pair<long,long> >::TransferBinaryIn: first then second through the
// inlined CMemoryDataInputStream::Read -- straight from the current chunk when 4 bytes remain, through the overflow
// reader otherwise, and not at all when position + 4 leaves [0, 0x7FFFFFFF].
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
typedef long long __int64;
#endif

struct CMemoryDataInputStream {
    void* __vftable;
    long  position;
    long  sourceLength;
    long  chunkBase;                                   // current chunk read pointer
    long  chunkPosition;
    long  chunkLength;                                 // bytes left in the chunk
    void* StreamSource;
};

static int g_overflows = 0; static long g_overflowValue = 0;
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++g_overflows; std::memcpy(buffer, &g_overflowValue, size); position += size;
    }
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

template <class T1, class T2> struct pair { T1 first; T2 second; };

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<pair<long, long> > {
    pair<long, long> DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, pair<long, long>* value);
};

void CPersistTraits<pair<long, long> >::TransferBinaryIn(CMemoryDataInputStream* stream, pair<long, long>* value)
{
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->first);
    longTraits.TransferBinaryIn(stream, &value->second);
}

int main()
{
    long data[4] = { 11, 22, 33, 44 };
    const long W = (long)sizeof(long);                  // 4 under VC7.1, 8 on a 64-bit host
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.position = 100; s.chunkBase = (long)data; s.chunkLength = 4 * W;
    CPersistTraits<pair<long, long> > traits;
    pair<long, long> r;
    traits.TransferBinaryIn(&s, &r);
    int ok = r.first == 11 && r.second == 22 && s.position == 100 + 2 * W && s.chunkLength == 2 * W && s.chunkBase == (long)(data + 2) && g_overflows == 0;
    s.chunkLength = W; g_overflowValue = 99;           // second value crosses into the next chunk
    traits.TransferBinaryIn(&s, &r);
    ok = ok && r.first == 33 && r.second == 99 && g_overflows == 1 && s.position == 100 + 4 * W;
    s.position = 0x7ffffffe; s.chunkLength = 2 * W;    // past the 2 GB guard: the stream is not touched
    traits.TransferBinaryIn(&s, &r);
    ok = ok && s.position == 0x7ffffffe && s.chunkLength == 2 * W && s.chunkBase == (long)(data + 3) && g_overflows == 1;
    if (!ok) { std::printf("00755d00_TEST FAIL\n"); return 1; }
    std::printf("00755d00_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x0046B0D6 CPersistTraits<pair<CDefIndex,CDefString> >::TransferBinaryIn: the CDefIndex through the
// inlined CMemoryDataInputStream::Read (straight from the current chunk when 4 bytes remain, through the overflow
// reader otherwise, not at all when position + 4 leaves [0, 0x7FFFFFFF]), then the def string through its own reader.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
typedef long long __int64;
#endif

class CMemoryDataInputStream { public:
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
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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

static int g_nameReads = 0; static CMemoryDataInputStream* g_nameStream = 0;
class CDefIndex { public: long Val; };
class CDefString {
public:
    long Index;
    void TransferBinaryIn(CMemoryDataInputStream& stream) { ++g_nameReads; g_nameStream = &stream; Index = 500 + g_nameReads; }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<pair<CDefIndex, CDefString> > {
    pair<CDefIndex, CDefString> DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, pair<CDefIndex, CDefString>* value);
};

void CPersistTraits<pair<CDefIndex, CDefString> >::TransferBinaryIn(CMemoryDataInputStream* stream, pair<CDefIndex, CDefString>* value)
{
    CDefIndex index;
    static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&index, sizeof(index));
    value->first = index;
    value->second.TransferBinaryIn(*stream);
}

int main()
{
    long data[4] = { 11, 22, 33, 44 };
    const long W = (long)sizeof(long);                  // 4 under VC7.1, 8 on a 64-bit host
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.position = 100; s.chunkBase = (long)data; s.chunkLength = 4 * W;
    CPersistTraits<pair<CDefIndex, CDefString> > traits;
    pair<CDefIndex, CDefString> r;
    traits.TransferBinaryIn(&s, &r);
    int ok = r.first.Val == 11 && r.second.Index == 501 && g_nameStream == &s && s.position == 100 + W && s.chunkLength == 3 * W && s.chunkBase == (long)(data + 1) && g_overflows == 0;
    s.chunkLength = 0; g_overflowValue = 99;           // the index crosses into the next chunk
    traits.TransferBinaryIn(&s, &r);
    ok = ok && r.first.Val == 99 && r.second.Index == 502 && g_overflows == 1 && s.position == 100 + 2 * W;
    s.position = 0x7ffffffe; s.chunkLength = 2 * W;    // past the 2 GB guard: the stream is not touched
    traits.TransferBinaryIn(&s, &r);
    ok = ok && s.position == 0x7ffffffe && s.chunkLength == 2 * W && s.chunkBase == (long)(data + 1) && g_overflows == 1 && g_nameReads == 3;
    if (!ok) { std::printf("0046b0d6_TEST FAIL\n"); return 1; }
    std::printf("0046b0d6_TEST PASS\n");
    return 0;
}
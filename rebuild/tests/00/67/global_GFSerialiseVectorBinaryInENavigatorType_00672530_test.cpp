// Behaviour test for 0x00672530 GFSerialiseVectorBinaryIn<ENavigatorType>: the count comes off the stream (fast
// (The model uses int for the 4-byte stream words: long is 8 bytes under x64 g++.)
// in-chunk path or the overflow path), the vector is resized to it with zero fill, and every element is read in order.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

struct CMemoryDataInputStreamReader {
    int  position;
    const unsigned char* chunkBase;
    int  chunkLength;
    int  overflowReads;
    int  overflowValue;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)   // models 0x00993CA0
    {
        ++overflowReads;
        std::memcpy(buffer, &overflowValue, size);
        position += size;
    }
    __forceinline void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned int)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace nav {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp  storage[16];
    void resize(size_type n, const _Tp& x)                  // models 0x00672660 (fixed capacity here)
    {
        size_type old = _M_finish - _M_start;
        for (size_type i = old; i < n; ++i) _M_start[i] = x;
        _M_finish = _M_start + n;
    }
    void resize(size_type n) { resize(n, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<ENavigatorType> {
    ENavigatorType DefaultVal;
    CPersistTraits() : DefaultVal(ENavigatorType()) {}
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, ENavigatorType* value)   // models 0x006725D0
    {
        int v;
        stream->Read(&v, sizeof(v));
        *value = (ENavigatorType)v;
    }
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, int* out)
{
    int value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, nav::vector<T>& out)
{
    CPersistTraits<T> traits;
    int count;
    ReadCount(stream, &count);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(&stream, &out[i]);
}

int main()
{
    int words[4] = { 3, 5, -1, 9 };
    CMemoryDataInputStreamReader s;
    s.position = 0; s.chunkBase = (const unsigned char*)words; s.chunkLength = sizeof(words);
    s.overflowReads = 0; s.overflowValue = 0;
    nav::vector<ENavigatorType> v;
    v._M_start = v._M_finish = v.storage; v._M_end_of_storage = v.storage + 16;
    GFSerialiseVectorBinaryIn<ENavigatorType>(s, v);
    if (v._M_finish - v._M_start != 3 || v[0] != 5 || v[1] != -1 || v[2] != 9 || s.position != 16 ||
        s.chunkLength != 0 || s.overflowReads != 0) {
        std::printf("00672530_TEST FAIL\n"); return 1;
    }
    // count through the overflow path; the elements then also overflow (empty chunk) and read the overflow value
    CMemoryDataInputStreamReader t;
    t.position = 0; t.chunkBase = 0; t.chunkLength = 0; t.overflowReads = 0; t.overflowValue = 2;
    nav::vector<ENavigatorType> w;
    w._M_start = w._M_finish = w.storage; w._M_end_of_storage = w.storage + 16;
    GFSerialiseVectorBinaryIn<ENavigatorType>(t, w);
    if (w._M_finish - w._M_start != 2 || w[0] != 2 || w[1] != 2 || t.overflowReads != 3) {
        std::printf("00672530_TEST FAIL\n"); return 1;
    }
    std::printf("00672530_TEST PASS\n");
    return 0;
}
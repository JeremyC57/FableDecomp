// Behaviour test for 0x00464D23 GFSerialiseVectorBinaryIn<float> (source body copied; stream, resize and overflow stubbed).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

struct CMemoryDataInputStream {
    void* __vftable;
    long  position;
    long  sourceLength;
    long  chunkBase;
    long  chunkPosition;
    long  chunkLength;
    void* StreamSource;
};
static int g_overflow = 0, g_resize = 0;


// (32-bit `long` of the source written as int so the g++ x64 build reads 4-byte counts)
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size);   // 0x00993CA0
    __forceinline void Read(void* buffer, int size)
    {
        __int64 newPosition = (__int64)(unsigned int)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);                  // 0x00442770 for float
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<float> {
    float DefaultVal;                                                     // never initialised here
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, float* out);
};

// The real CPersistTraits<float>::TransferBinaryIn body, out of line in the TU (GOTCHAS: empty traits objects).
__declspec(noinline) void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, float* out)
{
    stream->Read(out, sizeof(float));
}

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, int* out)
{
    int value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    int count;
    ReadCount(stream, &count);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(&stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<float>(CMemoryDataInputStreamReader& stream, std::vector<float>& out);

void CMemoryDataInputStreamReader::ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; memset(buffer, 0, size); }
namespace std {
template <> void vector<float>::resize(size_type n, const float& x)
{
    ++g_resize;
    _M_start = (float*)malloc(n * sizeof(float) + 1);
    for (size_type i = 0; i < n; ++i) _M_start[i] = x;
    _M_finish = _M_end_of_storage = _M_start + n;
}
}
int main()
{
    unsigned char buf[64];
    int count = 3;
    float vals[3] = { 1.5f, -2.25f, 8.0f };
    memcpy(buf, &count, 4);
    memcpy(buf + 4, vals, sizeof(vals));
    CMemoryDataInputStreamReader s;
    memset(&s, 0, sizeof s);
    s.chunkBase = (long)buf; s.chunkLength = 4 + (long)sizeof(vals);
    std::vector<float> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn<float>(s, v);
    if (g_resize != 1 || g_overflow != 0 || v._M_finish - v._M_start != 3) { printf("00464d23_TEST FAIL 1\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (v._M_start[i] != vals[i]) { printf("00464d23_TEST FAIL 2\n"); return 1; }
    if (s.position != 4 + (long)sizeof(vals) || s.chunkLength != 0 || s.chunkBase != (long)(buf + 4 + sizeof(vals))) { printf("00464d23_TEST FAIL 3\n"); return 1; }
    printf("00464d23_TEST PASS\n");
    return 0;
}
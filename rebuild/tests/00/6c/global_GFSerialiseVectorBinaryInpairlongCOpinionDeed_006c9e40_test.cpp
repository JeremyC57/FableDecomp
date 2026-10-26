// Behaviour test for 0x006C9E40 GFSerialiseVectorBinaryIn<pair<long, COpinionDeed*> > (source body copied; the 32-bit
// `long` written as int for the x64 g++ build; resize and the overflow path stubbed): read the count, resize, then read
// each element's handle and deed pointer in place.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
class CMemoryDataInputStream { public:
    void* __vftable;
    int   position;
    int   sourceLength;
    char* chunkBase;
    int   chunkPosition;
    int   chunkLength;
    void* StreamSource;
};
static int g_overflow = 0, g_resize = 0;


struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, int size)
    {
        __int64 newPosition = (__int64)(unsigned int)(position + size);
        if (!IsValidPosition(newPosition))
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

class COpinionDeed;

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x006C9D50 for pair<int, COpinionDeed*>
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;

template <> struct CPersistTraits<int> {                  // scalar traits: empty, no stored default
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, int* out) const;               // 0x0040FE60
};

template <> struct CPersistTraits<COpinionDeed*> {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, COpinionDeed** out) const;      // 0x006C88D0
};

__declspec(noinline) void CPersistTraits<int>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, int* out) const
{
    stream.Read(out, sizeof(int));
}
__declspec(noinline) void CPersistTraits<COpinionDeed*>::TransferBinaryIn(CMemoryDataInputStreamReader& stream, COpinionDeed** out) const
{
    stream.Read(out, sizeof(COpinionDeed*));
}

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    std::pair<T1, T2> DefaultVal;
    CPersistTraits() : DefaultVal(T1(), T2()) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, std::pair<T1, T2>* out) const
    {
        CPersistTraits<T1> firstTraits;
        firstTraits.TransferBinaryIn(stream, &out->first);
        CPersistTraits<T2> secondTraits;
        secondTraits.TransferBinaryIn(stream, &out->second);
    }
};

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
        traits.TransferBinaryIn(stream, &out[i]);
}


void CMemoryDataInputStreamReader::ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; memset(buffer, 0, size); }
namespace std {
template <> void vector<pair<int, COpinionDeed*> >::resize(unsigned int newSize)
{
    ++g_resize;
    _M_start = (pair<int, COpinionDeed*>*)malloc(newSize * sizeof(pair<int, COpinionDeed*>) + 1);
    _M_finish = _M_end_of_storage = _M_start + newSize;
}
}

int main()
{
    typedef std::pair<int, COpinionDeed*> E;
    unsigned char buf[256]; unsigned char* p = buf;
    int count = 3; memcpy(p, &count, 4); p += 4;
    int deeds[3];
    for (int k = 0; k < 3; ++k) {
        int h = 700 + k; memcpy(p, &h, 4); p += 4;
        COpinionDeed* d = (COpinionDeed*)&deeds[k]; memcpy(p, &d, sizeof(d)); p += sizeof(d);
    }
    CMemoryDataInputStreamReader stream; memset(&stream, 0, sizeof(stream));
    stream.chunkBase = (char*)buf; stream.chunkLength = (int)(p - buf);
    std::vector<E> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn(stream, v);
    int ok = 1;
    if (g_resize != 1 || g_overflow != 0 || v._M_finish - v._M_start != 3) ok = 0;
    for (int k = 0; k < 3 && ok; ++k) if (v._M_start[k].first != 700 + k || v._M_start[k].second != (COpinionDeed*)&deeds[k]) ok = 0;
    if (stream.position != (int)(p - buf) || stream.chunkLength != 0) ok = 0;
    printf(ok ? "006c9e40_TEST PASS\n" : "006c9e40_TEST FAIL\n");
    return ok ? 0 : 1;
}
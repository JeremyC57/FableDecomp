// Behaviour test for 0x00619620 GFSerialiseVectorBinaryIn<long>(stream, vector): read the element count (inline checked
// 4-byte read: in-chunk memcpy or ReadWithSrcChunkOverflow 0x00993CA0), resize the vector (resize(n, T()) 0x00619000),
// then read every element through CPersistTraits<long>::TransferBinaryIn (0x006148A0) -- callees modelled.
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
typedef __int64 s64;
#else
typedef unsigned long long u64;
typedef long long s64;
#endif
typedef int slong;   // the retail 32-bit `long` (g++ x86-64 long is 8 bytes)

struct CMemoryDataInputStreamReader {
    long                 position;
    const unsigned char* chunkBase;
    long                 chunkLength;
    const unsigned char* overflowSource;   // where ReadWithSrcChunkOverflow continues (model only)
    int                  overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        std::memcpy(buffer, overflowSource, size);
        overflowSource += size;
        position += size;
    }
    static bool IsValidPosition(s64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        s64 newPosition = (s64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
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

template <class T>
class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value)
    {
        unsigned int old = (unsigned int)(_M_finish - _M_start);
        if (newSize > (unsigned int)(_M_end_of_storage - _M_start)) {
            T* s = (T*)std::malloc(newSize * sizeof(T));
            for (unsigned int i = 0; i < old; ++i) s[i] = _M_start[i];
            std::free(_M_start);
            _M_start = s;
            _M_end_of_storage = s + newSize;
        }
        for (unsigned int i = old; i < newSize; ++i) _M_start[i] = value;
        _M_finish = _M_start + newSize;
    }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const { stream.Read(out, sizeof(T)); }
};

inline void ReadCount(CMemoryDataInputStreamReader& stream, slong* out)
{
    slong value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CRetailVector<T>& out)
{
    slong count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (slong i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, &out[i]);
    }
}

typedef slong E;

int main()
{
    int ok = 1;
    unsigned char buf[64];
    slong count = 3;
    E vals[3];
    vals[0] = (E)7; vals[1] = (E)-2; vals[2] = (E)0x12345678;
    std::memcpy(buf, &count, 4);
    std::memcpy(buf + 4, vals, sizeof(vals));
    CMemoryDataInputStreamReader s;
    s.position = 0; s.chunkBase = buf; s.chunkLength = (long)(4 + sizeof(vals)); s.overflowSource = 0; s.overflowCalls = 0;
    CRetailVector<E> v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn(s, v);
    if (v._M_finish - v._M_start != 3 || v[0] != vals[0] || v[1] != vals[1] || v[2] != vals[2] ||
        s.position != (long)(4 + sizeof(vals)) || s.chunkLength != 0 || s.overflowCalls != 0) ok = 0;
    // the count straddles the chunk end: it comes through the overflow reader, then the elements too
    CMemoryDataInputStreamReader t;
    slong two = 2;
    std::memcpy(buf, &two, 4);
    std::memcpy(buf + 4, vals, 2 * sizeof(E));
    t.position = 0; t.chunkBase = buf; t.chunkLength = 2; t.overflowSource = buf; t.overflowCalls = 0;
    GFSerialiseVectorBinaryIn(t, v);
    if (v._M_finish - v._M_start != 2 || v[0] != vals[0] || v[1] != vals[1] || t.overflowCalls != 3) ok = 0;
    // a zero count shrinks the vector and reads nothing more
    slong zero = 0;
    std::memcpy(buf, &zero, 4);
    CMemoryDataInputStreamReader z;
    z.position = 0; z.chunkBase = buf; z.chunkLength = 64; z.overflowSource = 0; z.overflowCalls = 0;
    GFSerialiseVectorBinaryIn(z, v);
    if (v._M_finish != v._M_start || z.position != 4) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("00619620_TEST FAIL\n"); return 1; }
    std::printf("00619620_TEST PASS\n");
    return 0;
}
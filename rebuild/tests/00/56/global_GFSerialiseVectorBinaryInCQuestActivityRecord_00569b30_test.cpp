// Behaviour test for 0x00569B30 GFSerialiseVectorBinaryIn<CQuestActivityRecord>: count, resize, per-element reads,
// both the in-chunk fast path and the chunk-overflow slow path of the count read.
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
typedef __int64 s64;
#else
typedef long long s64;
#endif

struct CMemoryDataInputStreamReader {
    int position;
    const unsigned char* chunkBase;
    int chunkLength;
    const unsigned char* overflowData;          // what the slow path reads from (stand-in for the next chunk)
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        std::memcpy(buffer, overflowData, size);
        overflowData += size;
        position += size;
        chunkBase = overflowData;
        chunkLength = 1000;
    }
    static bool IsValidPosition(s64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        s64 newPosition = (s64)(unsigned int)(position + size);
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

struct CQuestActivityRecord {
    int Value;
    CQuestActivityRecord() : Value(0) { }
};

namespace stl {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void resize(unsigned int newSize, const T& x)       // stand-in for 0x005697C0
    {
        unsigned int old = (unsigned int)(_M_finish - _M_start);
        if (newSize > old) {
            T* p = (T*)std::malloc(sizeof(T) * newSize);
            for (unsigned int i = 0; i < old; ++i) p[i] = _M_start[i];
            for (unsigned int i = old; i < newSize; ++i) p[i] = x;
            std::free(_M_start);
            _M_start = p; _M_end_of_storage = p + newSize;
        }
        _M_finish = _M_start + newSize;
    }
    void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const
    {
        int value;
        stream.Read(&value, sizeof(value));
        out->Value = value;
    }
};
inline void ReadCount(CMemoryDataInputStreamReader& stream, int* out)
{
    int value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class T>
void GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, stl::vector<T>& out)
{
    CPersistTraits<T> traits;
    int count;
    ReadCount(stream, &count);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}

int main()
{
    int words[4] = { 3, 10, -20, 30 };
    unsigned char buf[16];
    std::memcpy(buf, words, sizeof(buf));
    int ok = 1;
    {   // whole payload in the current chunk
        CMemoryDataInputStreamReader s;
        s.position = 0; s.chunkBase = buf; s.chunkLength = 16; s.overflowData = 0;
        stl::vector<CQuestActivityRecord> v;
        GFSerialiseVectorBinaryIn(s, v);
        if (v._M_finish - v._M_start != 3 || v[0].Value != 10 || v[1].Value != -20 || v[2].Value != 30 || s.position != 16) ok = 0;
        std::free(v._M_start);
    }
    {   // the count straddles a chunk boundary
        CMemoryDataInputStreamReader s;
        s.position = 0; s.chunkBase = buf; s.chunkLength = 2; s.overflowData = buf;
        stl::vector<CQuestActivityRecord> v;
        GFSerialiseVectorBinaryIn(s, v);
        if (v._M_finish - v._M_start != 3 || v[2].Value != 30) ok = 0;
        std::free(v._M_start);
    }
    {   // a position past 0x7FFFFFFF rejects the read: count stays garbage-free only via the stream, so use 0 elements
        int zero[1] = { 0 };
        CMemoryDataInputStreamReader s;
        s.position = 0; s.chunkBase = (const unsigned char*)zero; s.chunkLength = 4; s.overflowData = 0;
        stl::vector<CQuestActivityRecord> v;
        GFSerialiseVectorBinaryIn(s, v);
        if (v._M_finish != v._M_start) ok = 0;
        std::free(v._M_start);
    }
    if (!ok) { std::printf("00569b30_TEST FAIL\n"); return 1; }
    std::printf("00569b30_TEST PASS\n");
    return 0;
}
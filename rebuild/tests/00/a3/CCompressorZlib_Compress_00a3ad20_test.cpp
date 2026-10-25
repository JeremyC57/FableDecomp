#include <cstdio>
#include <stdlib.h>

#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

#pragma pack(push, 1)
class CCompressionInfo {
public:
    unsigned int  UncompressedSize;
    unsigned int  CompressedSize;
    unsigned char Method;
};
#pragma pack(pop)

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int n) { return _M_start[n]; }
    // test version of resize(n, x) for a POD element
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        unsigned int old = size();
        if (__new_size > (unsigned int)(_M_end_of_storage - _M_start)) {
            _Tp* p = (_Tp*)malloc(__new_size * sizeof(_Tp));
            for (unsigned int i = 0; i < old; ++i)
                p[i] = _M_start[i];
            free(_M_start);
            _M_start = p;
            _M_end_of_storage = p + __new_size;
        }
        for (unsigned int i = old; i < __new_size; ++i)
            _M_start[i] = __x;
        _M_finish = _M_start + __new_size;
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    T* GetAsCArray() { return this->size() != 0 ? &(*this)[0] : (T*)this; }
};

class CACompressor {
public:
    virtual ~CACompressor() { }
    virtual void Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize,
                          CCompressionInfo& info) = 0;
};

static unsigned long g_srcSize = 0, g_destSize = 0;
static const void* g_src = 0;
static void* g_dest = 0;

class CCompressorZlib : public CACompressor {
public:
    long Compress(const void* data, long count, long elementSize, CArray<unsigned char>& output);
    // test stand-in for the slot-5 compressor: "compresses" to half the size + 1 and fills the output
    virtual void Compress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize,
                          CCompressionInfo& info)
    {
        g_src = source;
        g_srcSize = sourceSize;
        g_dest = dest;
        g_destSize = destSize;
        info.UncompressedSize = (unsigned int)sourceSize;
        info.CompressedSize = (unsigned int)(sourceSize / 2 + 1);
        for (unsigned long i = 0; i < info.CompressedSize; ++i)
            ((unsigned char*)dest)[i] = (unsigned char)(0xA0 + i);
    }
};

long CCompressorZlib::Compress(const void* data, long count, long elementSize, CArray<unsigned char>& output)
{
    i64 maxSize = (i64)count * elementSize * 2 + 13;
    if (maxSize < 0 || maxSize > 0x7fffffff)
        return 0;
    output.resize(count * elementSize * 2 + 13);
    CCompressionInfo info;
    Compress(data, count * elementSize, output.GetAsCArray(), count * elementSize * 2 + 13, info);
    output.resize(info.CompressedSize);
    return info.CompressedSize;
}

int main()
{
    CCompressorZlib z;
    CArray<unsigned char> out;
    short input[10];
    for (int i = 0; i < 10; ++i)
        input[i] = (short)i;
    long r = z.Compress(input, 10, 2, out);
    if (r != 11 || out.size() != 11 || g_src != input || g_srcSize != 20 || g_destSize != 53 || out[0] != 0xA0 || out[10] != 0xAA)
        { std::printf("00a3ad20_TEST FAIL\n"); return 1; }
    CArray<unsigned char> out2;
    g_srcSize = 12345;
    if (z.Compress(input, 0x40000000, 1, out2) != 0 || out2.size() != 0 || g_srcSize != 12345)
        { std::printf("00a3ad20_TEST FAIL\n"); return 1; }
    if (z.Compress(input, -10, 1, out2) != 0 || out2.size() != 0)
        { std::printf("00a3ad20_TEST FAIL\n"); return 1; }
    std::printf("00a3ad20_TEST PASS\n");
    return 0;
}
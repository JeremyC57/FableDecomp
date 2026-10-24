#include <cstdio>
#include <cstdlib>
#include <cstring>

// Model of 0x00412D20 GFSerialiseVectorBinaryIn<unsigned long>: read the count with the inline checked 4-byte read
// (chunk fast path, else ReadWithSrcChunkOverflow 0x00993CA0), resize the vector with zeros, then read every element
// through CPersistTraits<unsigned long>::TransferBinaryIn. The stream is a two-chunk byte-buffer stand-in.
struct CMemoryDataInputStreamReader {
    long position;
    const unsigned char* chunkBase;
    long chunkLength;
    const unsigned char* overflowBase;            // where the slow path continues
    int overflowReads;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowReads;
        long fromChunk = chunkLength;
        std::memcpy(buffer, chunkBase, fromChunk);
        std::memcpy(buffer + fromChunk, overflowBase, size - fromChunk);
        chunkBase = overflowBase + (size - fromChunk);
        chunkLength = 0x1000;
        position += size;
    }
    static bool IsValidPosition(double newPosition) { return newPosition >= 0 && newPosition <= 2147483647.0; }
    void Read(void* buffer, long size)
    {
        double newPosition = (double)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
template <class T> class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value)
    {
        std::free(_M_start);
        _M_start = (T*)std::malloc((newSize + 1) * sizeof(T));
        for (unsigned int i = 0; i < newSize; ++i)
            _M_start[i] = value;
        _M_finish = _M_end_of_storage = _M_start + newSize;
    }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const
    {
        unsigned int value;
        stream.Read(&value, 4);
        *out = value;
    }
};
static void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    int value;
    stream.Read(&value, 4);
    *out = value;
}
template <class T> void GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CRetailVector<T>& out)
{
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (long i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, &out[i]);
    }
}

static void Put32(unsigned char* p, unsigned int v)
{
    p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24);
}

int main()
{
    unsigned char data[16];
    unsigned char more[16];
    Put32(data, 3);
    Put32(data + 4, 0x11111111);
    Put32(data + 8, 0xDEADBEEF);
    Put32(more, 0x00000042);                      // third element comes from the overflow chunk
    CMemoryDataInputStreamReader stream;
    stream.position = 0;
    stream.chunkBase = data;
    stream.chunkLength = 12;
    stream.overflowBase = more;
    stream.overflowReads = 0;
    CRetailVector<unsigned long> out;
    out._M_start = out._M_finish = out._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn(stream, out);
    if (out._M_finish - out._M_start != 3 || out[0] != 0x11111111 || out[1] != 0xDEADBEEFul || out[2] != 0x42 ||
        stream.position != 16 || stream.overflowReads != 1) {
        std::printf("00412d20_TEST FAIL\n");
        return 1;
    }
    unsigned char zero[4];
    Put32(zero, 0);
    stream.chunkBase = zero;
    stream.chunkLength = 4;
    GFSerialiseVectorBinaryIn(stream, out);
    if (out._M_finish != out._M_start || stream.position != 20) {
        std::printf("00412d20_TEST FAIL\n");
        return 1;
    }
    std::free(out._M_start);
    std::printf("00412d20_TEST PASS\n");
    return 0;
}
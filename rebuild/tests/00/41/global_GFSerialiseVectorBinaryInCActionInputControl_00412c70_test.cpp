#include <cstdio>
#include <cstdlib>
#include <cstring>

// Model of 0x00412C70 GFSerialiseVectorBinaryIn<CActionInputControl>: read the count with the inline checked 4-byte
// read (chunk fast path, else ReadWithSrcChunkOverflow 0x00993CA0), resize with zeroed bindings, then let every element
// read itself (CActionInputControl::TransferBinaryIn 0x00410120: five longs, then the 2D direction). The stream is a
// two-chunk byte-buffer stand-in.
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
class C2DVector { public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CActionInputControl {
public:
    int GameAction;
    int ControllerType;
    int KeyboardKey;
    int XboxButton;
    int MouseButton;
    C2DVector ControlDirection;
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0), ControlDirection(0.0f, 0.0f) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream)
    {
        stream.Read(&GameAction, 4);
        stream.Read(&ControllerType, 4);
        stream.Read(&KeyboardKey, 4);
        stream.Read(&XboxButton, 4);
        stream.Read(&MouseButton, 4);
        stream.Read(&ControlDirection.X, 4);
        stream.Read(&ControlDirection.Y, 4);
    }
};
template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const { out->TransferBinaryIn(stream); }
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

static void PutF(unsigned char* p, float f)
{
    std::memcpy(p, &f, 4);
}

int main()
{
    unsigned char data[64];
    unsigned char more[16];
    Put32(data, 2);
    for (int e = 0; e < 2; ++e) {
        unsigned char* p = data + 4 + e * 28;
        for (int k = 0; k < 5; ++k)
            Put32(p + 4 * k, 10 * (e + 1) + k);
        if (e == 0) {
            PutF(p + 20, 0.25f);
            PutF(p + 24, -0.5f);
        }
    }
    PutF(more, 1.5f);                              // the second element's direction crosses into the overflow chunk
    PutF(more + 4, 2.0f);
    CMemoryDataInputStreamReader stream;
    stream.position = 0;
    stream.chunkBase = data;
    stream.chunkLength = 4 + 28 + 20;
    stream.overflowBase = more;
    stream.overflowReads = 0;
    CRetailVector<CActionInputControl> out;
    out._M_start = out._M_finish = out._M_end_of_storage = 0;
    GFSerialiseVectorBinaryIn(stream, out);
    if (out._M_finish - out._M_start != 2 || out[0].GameAction != 10 || out[0].MouseButton != 14 ||
        out[0].ControlDirection.X != 0.25f || out[0].ControlDirection.Y != -0.5f || out[1].GameAction != 20 ||
        out[1].XboxButton != 23 || out[1].ControlDirection.X != 1.5f || out[1].ControlDirection.Y != 2.0f ||
        stream.position != 60 || stream.overflowReads != 1) {
        std::printf("00412c70_TEST FAIL\n");
        return 1;
    }
    unsigned char zero[4];
    Put32(zero, 0);
    stream.chunkBase = zero;
    stream.chunkLength = 4;
    GFSerialiseVectorBinaryIn(stream, out);
    if (out._M_finish != out._M_start || stream.position != 64) {
        std::printf("00412c70_TEST FAIL\n");
        return 1;
    }
    std::free(out._M_start);
    std::printf("00412c70_TEST PASS\n");
    return 0;
}
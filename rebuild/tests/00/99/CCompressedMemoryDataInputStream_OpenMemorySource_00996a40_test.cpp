#include <cstdio>
// Behaviour test for CCompressedMemoryDataInputStream @ 0x00996a40 (constructor / OpenMemorySource logic).
static int g_live = 0, g_created = 0;
struct Zlib {
    const void* source; long size;
    Zlib() : source(0), size(0) { ++g_live; ++g_created; }
    virtual ~Zlib() { --g_live; }
    void SetStreamInput(const void* s, long n) { source = s; size = n; }
};
template<class T> struct Scoped {
    T* ptr;
    Scoped() : ptr(0) { }
    ~Scoped() { if (ptr) delete ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; if (old) delete old; }
    T* operator->() const { return ptr; }
};
struct Stream {
    unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    unsigned char* Buffer; long BufferSize; Scoped<Zlib> Compressor;
    void ResetStream() { StreamPos = 0; SourceChunkStreamPos = 0; CurrentSourcePtr = 0; SourceChunkBytesRemaining = 0; StreamSize = -1; }
    Stream(const void* source, long sourceSize, long bufferSize)
        : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
    {
        Compressor.reset(new Zlib);
        Compressor->SetStreamInput(source, sourceSize);
        Buffer = new unsigned char[bufferSize];
        BufferSize = bufferSize;
        ResetStream();
    }
    ~Stream() { delete[] Buffer; }
    void OpenMemorySource(const void* source, long sourceSize, long bufferSize)
    {
        if (!Compressor.ptr) Compressor.reset(new Zlib);
        if (Buffer && BufferSize != bufferSize) { delete[] Buffer; Buffer = 0; }
        if (!Buffer) { Buffer = new unsigned char[bufferSize]; BufferSize = bufferSize; }
        Compressor->SetStreamInput(source, sourceSize);
        ResetStream();
    }
};
static int Fail() { std::printf("00996a40_TEST FAIL\n"); return 1; }
int main()
{
    char src[16]; char src2[8];
    {
        Stream s(src, 16, 64);
        if (g_created != 1 || s.Compressor->source != src || s.Compressor->size != 16) return Fail();
        if (s.Buffer == 0 || s.BufferSize != 64 || s.StreamSize != -1 || s.StreamPos != 0) return Fail();
        unsigned char* before = s.Buffer;
        s.StreamPos = 9; s.OpenMemorySource(src2, 8, 64);
        if (s.Buffer != before || g_created != 1 || s.Compressor->source != src2 || s.StreamPos != 0) return Fail();
        s.OpenMemorySource(src, 16, 128);
        if (s.BufferSize != 128 || s.Buffer == 0) return Fail();
        s.Compressor.reset(0);
        s.OpenMemorySource(src, 4, 128);
        if (g_created != 2 || g_live != 1 || s.Compressor->size != 4) return Fail();
    }
    if (g_live != 0) return Fail();
    std::printf("00996a40_TEST PASS\n");
    return 0;
}
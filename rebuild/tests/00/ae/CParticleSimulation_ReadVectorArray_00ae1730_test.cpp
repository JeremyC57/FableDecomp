// Behaviour test for CParticleSimulation::ReadVectorArray (0x00AE1730): count packed C3DVectors are read from the stream
// (inline fast path or the chunk-overflow path) and widened into C4DVectors with the given w.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class C3DVector { public: float X, Y, Z; };
class C4DVector { public: float X, Y, Z, W; void Initialise(const C3DVector& v, float w) { X = v.X; Y = v.Y; Z = v.Z; W = w; } };

class CDataInputStream {
public:
    unsigned int StreamPos;
    int StreamSize;
    const unsigned char* CurrentSourcePtr;
    int SourceChunkBytesRemaining;
    const unsigned char* Whole;                       // test-only: the full source for the overflow path
    int OverflowCalls;
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++OverflowCalls;
        std::memcpy(buffer, Whole + StreamPos, size);
        StreamPos += size;
    }
    void Read(void* buffer, int size)
    {
        long long np = (long long)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

template <class T> class CArray {
public:
    T* Start; T* Finish; T* End;
    explicit CArray(unsigned int n) { Start = n ? (T*)std::malloc(n * sizeof(T)) : 0; Finish = End = Start + n; }
    ~CArray() { if (Start) std::free(Start); }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    T& operator[](unsigned int i) { return Start[i]; }
    T* GetAsCArray() { return size() > 0 ? Start : (T*)this; }
    unsigned int GetByteSize() const { return size() * sizeof(T); }
};

static void ReadVectorArray(C4DVector* vectors, unsigned int count, CDataInputStream& in, float w)
{
    CArray<C3DVector> temp(count);
    C3DVector* data = temp.GetAsCArray();
    int bytes = temp.GetByteSize();
    if (bytes > 0) in.Read(data, bytes);
    for (unsigned int i = 0; i < count; ++i)
        vectors[i].Initialise(temp[i], w);
}

static int fail(int k) { std::printf("00ae1730_TEST FAIL %d\n", k); return 1; }

int main()
{
    float src[3 * 7];
    int i;
    for (i = 0; i < 21; ++i) src[i] = (float)(i + 1);
    CDataInputStream in;
    in.StreamPos = 0; in.StreamSize = sizeof(src); in.CurrentSourcePtr = (const unsigned char*)src;
    in.SourceChunkBytesRemaining = sizeof(src); in.Whole = (const unsigned char*)src; in.OverflowCalls = 0;
    C4DVector out[8];
    out[7].X = -5.0f;
    ReadVectorArray(out, 7, in, 1.0f);       // seven elements: the unrolled four plus the remainder three
    for (i = 0; i < 7; ++i)
        if (out[i].X != (float)(3 * i + 1) || out[i].Y != (float)(3 * i + 2) || out[i].Z != (float)(3 * i + 3) || out[i].W != 1.0f) return fail(1);
    if (out[7].X != -5.0f || in.StreamPos != sizeof(src) || in.OverflowCalls != 0) return fail(2);
    // the overflow path: the chunk holds fewer bytes than requested
    in.StreamPos = 0; in.CurrentSourcePtr = (const unsigned char*)src; in.SourceChunkBytesRemaining = 8;
    ReadVectorArray(out, 2, in, 0.0f);
    if (in.OverflowCalls != 1 || out[1].Z != 6.0f || out[1].W != 0.0f) return fail(3);
    // zero elements read nothing
    ReadVectorArray(out, 0, in, 2.0f);
    if (in.StreamPos != 24 || out[0].W != 0.0f) return fail(4);
    std::printf("00ae1730_TEST PASS\n");
    return 0;
}
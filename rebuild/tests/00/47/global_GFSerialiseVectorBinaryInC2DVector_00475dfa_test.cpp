// Behaviour test for 0x00475DFA GFSerialiseVectorBinaryIn<C2DVector> (self-contained C++03 model of the shared body: the count
// through the inline bounds-checked chunk read, resize(n), then every element read as two floats through Read2DVector).
#include <stdio.h>
#include <string.h>
#include <vector>

#ifdef _MSC_VER
typedef __int64 TestInt64;
#else
typedef long long TestInt64;
#endif

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }

class C2DVector {
public:
    float X, Y;
    C2DVector() : X(0.0f), Y(0.0f) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class CMemoryDataInputStream {                // CDataInputStream +4..+0x14 and the inline bounds-checked Read
public:
    unsigned long StreamPos;
    const unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    int OverflowCalls;
    CMemoryDataInputStream() : StreamPos(0), CurrentSourcePtr(0), SourceChunkBytesRemaining(0), OverflowCalls(0) { }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++OverflowCalls; memset(buffer, 0, size); StreamPos += size; }
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        TestInt64 newPosition = (TestInt64)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadSLONG() { int value; Read(&value, sizeof(value)); return value; }
    C2DVector Read2DVector() { float x, y; Read(&x, 4); Read(&y, 4); return C2DVector(x, y); }   // 0x00995030 model
};

static void GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is, std::vector<C2DVector>& vec)
{
    long count = is.ReadSLONG();
    vec.resize(count);
    for (long i = 0; i < count; ++i)
        vec[i] = is.Read2DVector();
}

int main()
{
    {
        unsigned char buf[4 + 3 * 8]; int n = 3; memcpy(buf, &n, 4);
        float f[6] = { 1.0f, 2.0f, -3.5f, 4.25f, 0.5f, 100.0f }; memcpy(buf + 4, f, sizeof(f));
        CMemoryDataInputStream in; in.CurrentSourcePtr = buf; in.SourceChunkBytesRemaining = sizeof(buf);
        std::vector<C2DVector> v(7);
        GFSerialiseVectorBinaryIn(in, v);
        Check(v.size() == 3, "resizes to the read count");
        Check(v[0].X == 1.0f && v[0].Y == 2.0f && v[1].X == -3.5f && v[1].Y == 4.25f && v[2].X == 0.5f && v[2].Y == 100.0f, "reads x then y per element");
        Check(in.StreamPos == sizeof(buf) && in.SourceChunkBytesRemaining == 0 && in.OverflowCalls == 0, "consumes count + 8 bytes per element");
    }
    {
        unsigned char buf[4]; int n = 0; memcpy(buf, &n, 4);
        CMemoryDataInputStream in; in.CurrentSourcePtr = buf; in.SourceChunkBytesRemaining = 4;
        std::vector<C2DVector> v(2);
        GFSerialiseVectorBinaryIn(in, v);
        Check(v.empty() && in.StreamPos == 4, "a zero count empties the vector");
    }
    {
        unsigned char buf[8]; int n = 2; memcpy(buf, &n, 4); memset(buf + 4, 0, 4);
        CMemoryDataInputStream in; in.CurrentSourcePtr = buf; in.SourceChunkBytesRemaining = 8;
        std::vector<C2DVector> v;
        GFSerialiseVectorBinaryIn(in, v);
        Check(v.size() == 2 && in.OverflowCalls == 3, "reads past the chunk go through the overflow path");
    }
    if (g_fail) return 1;
    printf("00475dfa_TEST PASS\n");
    return 0;
}
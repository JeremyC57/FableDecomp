#include <cstdio>
#include <cstring>
// Behaviour test for CLocalDetailPrimitiveMesh::Load (0x00BFA0F0): box, sphere, matrix and scale are read in order from the
// stream, through the in-chunk copy or the chunk-overflow path, and the function always returns true.
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
struct CDataInputStream {
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    long Overflows;
    const unsigned char* Base;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        Overflows++;
        std::memcpy(buffer, Base + StreamPos, size);
        StreamPos += size; CurrentSourcePtr = Base + StreamPos; SourceChunkBytesRemaining = 1000;
    }
    static bool IsValidPosition(Int64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* dst, long size)
    {
        Int64 np = (Int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    template <class T> void Read(T& value) { Read(&value, sizeof(T)); }
};
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };
struct C3DBoundingSphere { C3DVector Centre; float Radius; };
struct CMatrix3x4 { float E[12]; };
struct CLocalDetailPrimitiveMesh {
    C3DBoundingBox    BBox;
    C3DBoundingSphere BSphere;
    CMatrix3x4        Matrix;
    float             Scale;
    bool Load(CDataInputStream* in)
    {
        in->Read(BBox);
        in->Read(BSphere);
        in->Read(Matrix);
        float scale;
        in->Read(scale);
        Scale = scale;
        return true;
    }
};
int main()
{
    float data[29];
    int i;
    for (i = 0; i < 29; i++) data[i] = (float)(i + 1);
    for (int pass = 0; pass < 2; pass++) {
        CDataInputStream s;
        s.StreamPos = 0; s.StreamSize = sizeof(data); s.Base = (const unsigned char*)data; s.CurrentSourcePtr = s.Base;
        s.SourceChunkBytesRemaining = pass == 0 ? (long)sizeof(data) : 0x20;   // pass 1 overflows at the sphere
        s.Overflows = 0;
        CLocalDetailPrimitiveMesh m;
        std::memset(&m, 0, sizeof(m));
        if (!m.Load(&s)) { std::printf("00bfa0f0_TEST FAIL\n"); return 1; }
        if (m.BBox.MinCoord.X != 1.0f || m.BBox.MaxCoord.Z != 6.0f || m.BSphere.Centre.X != 7.0f || m.BSphere.Radius != 10.0f
            || m.Matrix.E[0] != 11.0f || m.Matrix.E[11] != 22.0f || m.Scale != 23.0f || s.StreamPos != 23 * 4
            || s.Overflows != (pass == 0 ? 0 : 1)) {
            std::printf("00bfa0f0_TEST FAIL\n"); return 1;
        }
    }
    std::printf("00bfa0f0_TEST PASS\n");
    return 0;
}

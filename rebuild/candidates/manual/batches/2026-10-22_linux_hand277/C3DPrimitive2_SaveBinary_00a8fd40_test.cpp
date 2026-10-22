// Behaviour test for C3DPrimitive2::SaveBinary @ 0x00A8FD40: the stream records every write; check order and values.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CVertexCompressionParams { float PosScale[4]; float PosBias[4]; };
static unsigned long g_log[256]; static int g_n = 0;
static void Log(unsigned long v) { if (g_n < 256) g_log[g_n++] = v; }
struct CDataOutputStream {
    void WriteData(const void* data, unsigned long size) { Log(0xD0); Log(size); Log((unsigned long)((const float*)data)[0]); }
    void WriteBool(bool v) { Log(0xB0); Log(v ? 1 : 0); }
    void WriteUChar(unsigned char v) { Log(0xC0); Log(v); }
    void WriteULong(unsigned long v) { Log(0xA0); Log(v); }
    void WriteLong(long v) { Log(0xA1); Log((unsigned long)v); }
    void WriteFloat(float v) { Log(0xF0); Log((unsigned long)v); }
    void Write3DVector(const C3DVector& v) { Log(0x3D); Log((unsigned long)v.X); }
    void WriteBuffer(const void* data, long size) { Log(0xBF); Log((unsigned long)((const unsigned char*)data)[0]); Log((unsigned long)size); }
};
static int g_locks = 0;
struct CVertexBufferWin32 {
    unsigned long m_handle; unsigned long ByteLength; unsigned long VertexSize; unsigned char VertexType;
    CVertexCompressionParams CompressionParams; unsigned char m_data[4];
    void* Lock(unsigned long, unsigned long, unsigned long) { ++g_locks; return m_data; }
    void Unlock() { --g_locks; }
};
struct CIndexBuffer {
    unsigned char m_data[4];
    unsigned char* Lock(unsigned long, unsigned long, unsigned long) { ++g_locks; return m_data; }
    void Unlock() { --g_locks; }
};
struct C3DClothPrimitive { unsigned long id; void SaveBinary(CDataOutputStream&) const { Log(0xCC); Log(id); } };
template<class T> struct CCountedPointer { T* Data; void* Info; };
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    T& operator[](unsigned long i) const { return m_begin[i]; }
};
struct CStaticBlock { unsigned long StartIndex; unsigned long PrimitiveCount; bool DiffuseFlag; bool BumpFlag; unsigned char TextureStage; long MaterialIndex; };
struct CAnimatedBlock { unsigned long id; void SaveBinary(CDataOutputStream&) const { Log(0xAB); Log(id); } };
struct C3DPrimitive2 {
    long MaterialIndex; long RepeatingMeshReps; C3DVector BoundingCenter; float BoundingRadius; float AverageTextureStretch;
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;
    CVertexBufferWin32* VertexBuffer; CIndexBuffer* IndexBuffer;
    unsigned long StaticBlockCount; CStaticBlock* StaticBlocks; unsigned long AnimatedBlockCount; CAnimatedBlock* AnimatedBlocks;
    unsigned long VertexCount; unsigned long TriangleCount; unsigned long IndexCount; unsigned long InitFlags;
    int m_uploads;
    bool UploadToIndexBuffer(unsigned long) { ++m_uploads; Log(0x1B); return true; }
    void SaveBinary(CDataOutputStream& out);
};
void C3DPrimitive2::SaveBinary(CDataOutputStream& out)
{
    out.WriteLong(MaterialIndex);
    out.WriteLong(RepeatingMeshReps);
    out.Write3DVector(BoundingCenter);
    out.WriteFloat(BoundingRadius);
    out.WriteFloat(AverageTextureStretch);
    out.WriteULong(StaticBlockCount);
    out.WriteULong(AnimatedBlockCount);
    out.WriteULong(VertexCount);
    out.WriteULong(TriangleCount);
    out.WriteULong(IndexCount);
    out.WriteULong(InitFlags);
    out.WriteULong(StaticBlockCount);
    out.WriteULong(AnimatedBlockCount);
    for (unsigned long i = 0; i < StaticBlockCount; ++i) {
        const CStaticBlock& block = StaticBlocks[i];
        out.WriteULong(block.StartIndex);
        out.WriteULong(block.PrimitiveCount);
        out.WriteBool(block.DiffuseFlag);
        out.WriteUChar(block.TextureStage);
        out.WriteBool(block.BumpFlag);
        out.WriteLong(block.MaterialIndex);
    }
    for (unsigned long j = 0; j < AnimatedBlockCount; ++j)
        AnimatedBlocks[j].SaveBinary(out);
    long reps = RepeatingMeshReps;
    if (reps <= 1)
        reps = 1;
    CVertexBufferWin32* vb = VertexBuffer;
    CVertexCompressionParams params = vb->CompressionParams;
    out.WriteData(&params, sizeof(params));
    out.WriteULong(vb->VertexType);
    out.WriteULong(vb->VertexSize);
    void* vertices = vb->Lock(0, 0, 0);
    out.WriteBuffer(vertices, vb->ByteLength);
    vb->Unlock();
    UploadToIndexBuffer(0);
    CIndexBuffer* ib = IndexBuffer;
    unsigned char* indices = ib->Lock(0, 0, 0);
    out.WriteBuffer(indices, IndexCount * reps * sizeof(unsigned short));
    ib->Unlock();
    unsigned long clothCount = ClothPrimitives.size();
    out.WriteULong(clothCount);
    for (unsigned long c = 0; c < clothCount; ++c)
        ClothPrimitives[c].Data->SaveBinary(out);
}
int main()
{
    CStaticBlock sb[1]; sb[0].StartIndex = 5; sb[0].PrimitiveCount = 6; sb[0].DiffuseFlag = true; sb[0].BumpFlag = false; sb[0].TextureStage = 7; sb[0].MaterialIndex = 8;
    CAnimatedBlock ab[2]; ab[0].id = 21; ab[1].id = 22;
    C3DClothPrimitive cloth; cloth.id = 33;
    CCountedPointer<C3DClothPrimitive> cp[1]; cp[0].Data = &cloth; cp[0].Info = 0;
    CVertexBufferWin32 vb; std::memset(&vb, 0, sizeof(vb)); vb.ByteLength = 64; vb.VertexSize = 16; vb.VertexType = 3; vb.CompressionParams.PosScale[0] = 9.0f; vb.m_data[0] = 0x55;
    CIndexBuffer ib; ib.m_data[0] = 0x66;
    C3DPrimitive2 p; std::memset(&p, 0, sizeof(p));
    p.MaterialIndex = 1; p.RepeatingMeshReps = 3; p.BoundingCenter.X = 2.0f; p.BoundingRadius = 4.0f; p.AverageTextureStretch = 1.0f;
    p.ClothPrimitives.m_begin = cp; p.ClothPrimitives.m_end = cp + 1; p.ClothPrimitives.m_capacity = cp + 1;
    p.VertexBuffer = &vb; p.IndexBuffer = &ib; p.StaticBlockCount = 1; p.StaticBlocks = sb; p.AnimatedBlockCount = 2; p.AnimatedBlocks = ab;
    p.VertexCount = 10; p.TriangleCount = 11; p.IndexCount = 12; p.InitFlags = 13;
    CDataOutputStream out;
    p.SaveBinary(out);
    static const unsigned long expect[] = {
        0xA1,1, 0xA1,3, 0x3D,2, 0xF0,4, 0xF0,1, 0xA0,1, 0xA0,2, 0xA0,10, 0xA0,11, 0xA0,12, 0xA0,13, 0xA0,1, 0xA0,2,
        0xA0,5, 0xA0,6, 0xB0,1, 0xC0,7, 0xB0,0, 0xA1,8, 0xAB,21, 0xAB,22,
        0xD0,32,9, 0xA0,3, 0xA0,16, 0xBF,0x55,64, 0x1B, 0xBF,0x66,72, 0xA0,1, 0xCC,33 };
    int n = (int)(sizeof(expect) / sizeof(expect[0]));
    bool ok = g_n == n && g_locks == 0 && p.m_uploads == 1;
    for (int i = 0; ok && i < n; ++i) if (g_log[i] != expect[i]) ok = false;
    // reps clamp: 0 repeats still writes IndexCount * 1 * 2 bytes
    g_n = 0; p.RepeatingMeshReps = 0; p.StaticBlockCount = 0; p.AnimatedBlockCount = 0; p.ClothPrimitives.m_end = cp;
    p.SaveBinary(out);
    if (!(g_n > 4 && g_log[g_n - 3] == 24 && g_log[g_n - 2] == 0xA0 && g_log[g_n - 1] == 0)) ok = false;
    if (!ok) { std::printf("00a8fd40_TEST FAIL\n"); return 1; }
    std::printf("00a8fd40_TEST PASS\n");
    return 0;
}

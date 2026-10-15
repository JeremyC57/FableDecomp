// Behaviour test for 0x00ABBDF0 C3DMeshFileVerticesChunk::WriteToFile
#include <cstdio>
struct COut {
    float f[64]; unsigned long u[8]; int nf, nu;
    COut() : nf(0), nu(0) {}
    void WriteULong(unsigned long v) { u[nu++] = v; }
    void WriteFloat(float v) { f[nf++] = v; }
};
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};
struct C3DVector { float X, Y, Z; };
struct C3DVertex2 { C3DVector Position; C3DVector Normal; float U; float V; };
struct C3DMeshFileVerticesChunk {
    CArray<C3DVertex2> VertexList;
    void WriteToFile(COut& out) const
    {
        unsigned long count = VertexList.Size();
        out.WriteULong(count);
        for (unsigned long i = 0; i < count; ++i) {
            const C3DVertex2& v = VertexList[i];
            out.WriteFloat(v.Position.X); out.WriteFloat(v.Position.Y); out.WriteFloat(v.Position.Z);
            out.WriteFloat(v.Normal.X); out.WriteFloat(v.Normal.Y); out.WriteFloat(v.Normal.Z);
            out.WriteFloat(v.U); out.WriteFloat(v.V);
        }
    }
};
int main()
{
    C3DVertex2 vs[2] = { {{1, 2, 3}, {4, 5, 6}, 7, 8}, {{9, 10, 11}, {12, 13, 14}, 15, 16} };
    C3DMeshFileVerticesChunk c; c.VertexList.m_begin = vs; c.VertexList.m_end = vs + 2; c.VertexList.m_capacity = vs + 2;
    COut out; c.WriteToFile(out);
    bool ok = out.nu == 1 && out.u[0] == 2 && out.nf == 16;
    for (int i = 0; ok && i < 16; ++i) ok = out.f[i] == (float)(i + 1);
    if (!ok) { std::printf("00abbdf0_TEST FAIL\n"); return 1; }
    std::printf("00abbdf0_TEST PASS\n");
    return 0;
}
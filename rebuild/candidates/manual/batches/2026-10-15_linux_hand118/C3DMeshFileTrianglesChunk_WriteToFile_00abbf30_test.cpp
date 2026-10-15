// Behaviour test for 0x00ABBF30 C3DMeshFileTrianglesChunk::WriteToFile
#include <cstdio>
struct COut {
    unsigned long log[64]; int n;
    COut() : n(0) {}
    void WriteULong(unsigned long v) { log[n++] = v; }
    void WriteUShort(unsigned short v) { log[n++] = 0x10000u | v; }
};
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};
struct C3DMeshFileTriangle { unsigned short Index[3]; };
static int g_baseCalls = 0;
struct C3DMeshFileChunk { void WriteToFile(COut&, int&) const { ++g_baseCalls; } };
struct C3DMeshFileTrianglesChunk : C3DMeshFileChunk {
    CArray<C3DMeshFileTriangle> Triangles;
    void WriteToFile(COut& out, int& saver) const
    {
        unsigned long count = Triangles.Size();
        out.WriteULong(count);
        for (unsigned long i = 0; i < count; ++i) {
            const C3DMeshFileTriangle& t = Triangles[i];
            out.WriteUShort(t.Index[0]); out.WriteUShort(t.Index[1]); out.WriteUShort(t.Index[2]);
        }
        C3DMeshFileChunk::WriteToFile(out, saver);
    }
};
int main()
{
    C3DMeshFileTriangle tris[2] = { {{1, 2, 3}}, {{40000, 5, 6}} };
    C3DMeshFileTrianglesChunk c; c.Triangles.m_begin = tris; c.Triangles.m_end = tris + 2; c.Triangles.m_capacity = tris + 2;
    COut out; int saver = 0;
    c.WriteToFile(out, saver);
    unsigned long want[7] = { 2, 0x10001, 0x10002, 0x10003, 0x10000u | 40000, 0x10005, 0x10006 };
    bool ok = out.n == 7 && g_baseCalls == 1;
    for (int i = 0; ok && i < 7; ++i) ok = out.log[i] == want[i];
    C3DMeshFileTrianglesChunk e; e.Triangles.m_begin = e.Triangles.m_end = e.Triangles.m_capacity = 0;
    COut o2; e.WriteToFile(o2, saver);
    ok = ok && o2.n == 1 && o2.log[0] == 0 && g_baseCalls == 2;
    if (!ok) { std::printf("00abbf30_TEST FAIL\n"); return 1; }
    std::printf("00abbf30_TEST PASS\n");
    return 0;
}

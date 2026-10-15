#include <cstdio>
#include <cstring>
struct CCharString { const char* m_pText; };
struct C3DVector { float X, Y, Z; };
struct CChunkedFileSaver { int dummy; };
static char g_trace[256];
static void Log(const char* s) { std::strcat(g_trace, s); }
struct CDataOutputStream {
    void WriteULong(unsigned long v) { char b[32]; std::sprintf(b, "U%lu;", v); Log(b); }
    void WriteFloat(float v) { char b[32]; std::sprintf(b, "F%g;", v); Log(b); }
    void Write3DVector(const C3DVector& v) { char b[64]; std::sprintf(b, "V%g,%g,%g;", v.X, v.Y, v.Z); Log(b); }
    void WriteNullTerminatedString(const CCharString& s) { Log("S"); Log(s.m_pText); Log(";"); }
};
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
};
struct C3DPlane { C3DVector Normal; float Distance; };
struct C3DMeshFileConvexVolumeChunk {
    CCharString Name;
    CArray<C3DPlane> Planes;
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
    {
        out.WriteULong(1);
        out.WriteNullTerminatedString(Name);
        unsigned long count = Planes.Size();
        out.WriteULong(count);
        for (unsigned long i = 0; i < count; ++i) {
            const C3DPlane& plane = Planes[i];
            out.Write3DVector(plane.Normal);
            out.WriteFloat(plane.Distance);
        }
    }
};
int main()
{
    C3DPlane planes[2] = { { { 1, 0, 0 }, 5 }, { { 0, 1, 0 }, -2 } };
    C3DMeshFileConvexVolumeChunk c; c.Name.m_pText = "hull";
    c.Planes.m_begin = planes; c.Planes.m_end = planes + 2; c.Planes.m_capacity = planes + 2;
    CDataOutputStream out; CChunkedFileSaver saver; g_trace[0] = 0;
    c.WriteToFile(out, saver);
    if (std::strcmp(g_trace, "U1;Shull;U2;V1,0,0;F5;V0,1,0;F-2;") != 0) { std::printf("00aaa9a0_TEST FAIL\n"); return 1; }
    std::printf("00aaa9a0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
class CChunkedFileSaver { public: int dummy; };
static char g_trace[256];
static void Log(const char* s) { std::strcat(g_trace, s); }
class CDataOutputStream { public:
    void WriteLONG(long v) { char b[32]; std::sprintf(b, "L%ld;", v); Log(b); }
    void WriteULONG(unsigned long v) { char b[32]; std::sprintf(b, "U%lu;", v); Log(b); }
    void WriteUSHORT(unsigned short v) { char b[32]; std::sprintf(b, "W%u;", (unsigned)v); Log(b); }
    void WriteFLOAT(float v) { char b[32]; std::sprintf(b, "F%g;", v); Log(b); }
};
template <class T> class CArray { public:
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};
class CChunkedFileSuperChunk { public: void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const { Log("SUPER;"); } };
class C3DVertexBlend2 { public: unsigned long VertexIndex; float BlendValue; };
class C3DMeshFileGroupChunk : public CChunkedFileSuperChunk { public:
    long BoneIndex;
    CArray<C3DVertexBlend2> VertexList;
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
    {
        out.WriteLONG(BoneIndex);
        unsigned long count = VertexList.Size();
        out.WriteULONG(count);
        for (unsigned long i = 0; i < count; ++i) {
            const C3DVertexBlend2& blend = VertexList[i];
            out.WriteUSHORT((unsigned short)blend.VertexIndex);
            out.WriteFLOAT(blend.BlendValue);
        }
        CChunkedFileSuperChunk::WriteToFile(out, saver);
    }
};
int main()
{
    C3DVertexBlend2 v[2] = { { 70000, 0.25f }, { 3, 0.75f } };
    C3DMeshFileGroupChunk c; c.BoneIndex = 2; c.VertexList.m_pBegin = v; c.VertexList.m_pEnd = v + 2; c.VertexList.m_pEndOfStorage = v + 2;
    CDataOutputStream out; CChunkedFileSaver saver; g_trace[0] = 0;
    c.WriteToFile(out, saver);
    if (std::strcmp(g_trace, "L2;U2;W4464;F0.25;W3;F0.75;SUPER;") != 0) { std::printf("00abaee0_TEST FAIL\n"); return 1; }
    std::printf("00abaee0_TEST PASS\n");
    return 0;
}
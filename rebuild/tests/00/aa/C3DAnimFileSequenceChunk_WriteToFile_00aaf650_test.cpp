#include <cstdio>
#include <cstring>
// mock stream: records a trace of what was written
class CCharString { public: const char* m_pText; };
class CMatrix3x4 { public: float m_row[3][4]; };
class CChunkedFileSaver { public: int dummy; };
static char g_trace[512];
static void Log(const char* s) { std::strcat(g_trace, s); }
class CDataOutputStream { public:
    void WriteLONG(long v) { char b[32]; std::sprintf(b, "L%ld;", v); Log(b); }
    void WriteFLOAT(float v) { char b[32]; std::sprintf(b, "F%g;", v); Log(b); }
    void WriteUBYTE(unsigned char v) { char b[32]; std::sprintf(b, "B%d;", (int)v); Log(b); }
    void WriteMatrix3x4(const CMatrix3x4& m) { char b[32]; std::sprintf(b, "M%g;", m.m_row[0][0]); Log(b); }
    void WriteNullTerminatedString(const CCharString& s) { Log("S"); Log(s.m_pText); Log(";"); }
};
template <class T> class CArray { public:
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};
class CChunkedFileSuperChunk { public:
    void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const { Log("SUPER;"); }
};
class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:
    long BoneIndex; long ParentIndex; CCharString BoneName; float SamplesPerSecond;
    unsigned char BoneType; CArray<CMatrix3x4> Matrices;
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};
void C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteLONG(BoneIndex);
    out.WriteLONG(ParentIndex);
    out.WriteNullTerminatedString(BoneName);
    out.WriteFLOAT(SamplesPerSecond);
    unsigned long count = Matrices.Size();
    out.WriteLONG(count);
    for (unsigned long i = 0; i < count; ++i)
        out.WriteMatrix3x4(Matrices[i]);
    out.WriteUBYTE(BoneType);
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}
int main()
{
    CMatrix3x4 m[2];
    std::memset(m, 0, sizeof(m));
    m[0].m_row[0][0] = 1.0f; m[1].m_row[0][0] = 2.0f;
    C3DAnimFileSequenceChunk c;
    c.BoneIndex = 3; c.ParentIndex = -1; c.BoneName.m_pText = "spine"; c.SamplesPerSecond = 30.0f;
    c.BoneType = 2; c.Matrices.m_pBegin = m; c.Matrices.m_pEnd = m + 2; c.Matrices.m_pEndOfStorage = m + 2;
    CDataOutputStream out; CChunkedFileSaver saver;
    g_trace[0] = 0;
    c.WriteToFile(out, saver);
    if (std::strcmp(g_trace, "L3;L-1;Sspine;F30;L2;M1;M2;B2;SUPER;") != 0) {
        std::printf("00aaf650_TEST FAIL\n"); return 1;
    }
    std::printf("00aaf650_TEST PASS\n");
    return 0;
}
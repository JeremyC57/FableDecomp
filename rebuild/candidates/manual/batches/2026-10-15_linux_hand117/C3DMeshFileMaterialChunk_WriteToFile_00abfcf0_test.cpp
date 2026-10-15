#include <cstdio>
#include <cstring>
struct CCharString { const char* m_pText; };
struct CRGBColour { unsigned long m_argb; };
struct CChunkedFileSaver { int dummy; };
static char g_trace[256];
static void Log(const char* s) { std::strcat(g_trace, s); }
struct CDataOutputStream {
    void WriteLONG(long v) { char b[32]; std::sprintf(b, "L%ld;", v); Log(b); }
    void WriteBOOL(bool v) { Log(v ? "T;" : "N;"); }
    void WriteColour(const CRGBColour& c) { char b[32]; std::sprintf(b, "C%lx;", c.m_argb); Log(b); }
    void WriteFLOAT(float v) { char b[32]; std::sprintf(b, "F%g;", v); Log(b); }
    void WriteNullTerminatedString(const CCharString& s) { Log("S"); Log(s.m_pText); Log(";"); }
};
struct CChunkedFileSuperChunk { void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const { Log("SUPER;"); } };
struct C3DMeshFileMaterialChunk : CChunkedFileSuperChunk {
    long MaterialIndex; bool TwoSided; CRGBColour Ambient, Diffuse, Specular;
    float Shiny, ShinyStrength, Transparency; CCharString Name;
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
    {
        out.WriteNullTerminatedString(Name);
        out.WriteLONG(MaterialIndex);
        out.WriteBOOL(TwoSided);
        out.WriteColour(Ambient);
        out.WriteColour(Diffuse);
        out.WriteColour(Specular);
        out.WriteFLOAT(Shiny);
        out.WriteFLOAT(ShinyStrength);
        out.WriteFLOAT(Transparency);
        CChunkedFileSuperChunk::WriteToFile(out, saver);
    }
};
int main()
{
    C3DMeshFileMaterialChunk c; c.MaterialIndex = 4; c.TwoSided = true; c.Ambient.m_argb = 0x10; c.Diffuse.m_argb = 0x20;
    c.Specular.m_argb = 0x30; c.Shiny = 1.5f; c.ShinyStrength = 2.0f; c.Transparency = 0.5f; c.Name.m_pText = "skin";
    CDataOutputStream out; CChunkedFileSaver saver; g_trace[0] = 0;
    c.WriteToFile(out, saver);
    if (std::strcmp(g_trace, "Sskin;L4;T;C10;C20;C30;F1.5;F2;F0.5;SUPER;") != 0) { std::printf("00abfcf0_TEST FAIL\n"); return 1; }
    std::printf("00abfcf0_TEST PASS\n");
    return 0;
}

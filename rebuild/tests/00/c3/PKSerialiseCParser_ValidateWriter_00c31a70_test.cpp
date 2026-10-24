// Behaviour test for 0x00C31A70 PKSerialiseCParser::ValidateWriter: returns the writer; with none it logs the "Null writer" assertion line through the log sink and runs the trace helper
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_logged;
static int g_traces = 0;
typedef void (*PFNLOGSINK)(const char* text);
static void LogSink(const char* text) { g_logged += text; }
PFNLOGSINK g_pfnLogSink = 0;
struct CAssertString {
    std::string m_text;
    CAssertString* Append(const char* text) { m_text += text; return this; }
    CAssertString* AppendNumber(long value) { char b[16]; std::sprintf(b, "%ld", value); m_text += b; return this; }
};
struct CAssertTrace { CAssertTrace() { ++g_traces; } };
class PKSerialiseCWriter { public: int id; };
class PKSerialiseCParser {
public:
    long m_0;
    PKSerialiseCWriter* m_pWriter;
    PKSerialiseCWriter* ValidateWriter();
};
PKSerialiseCWriter* PKSerialiseCParser::ValidateWriter()
{
    if (!m_pWriter) {
        {
            CAssertString line;
            line.Append(".\\PKSerialiseCParser.cpp")->Append("(")->AppendNumber(17)->Append(") : ")->Append("Null writer")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_text.c_str());
        }
        CAssertTrace();
    }
    return m_pWriter;
}
int main()
{
    PKSerialiseCWriter w;
    PKSerialiseCParser p;
    p.m_pWriter = &w;
    if (p.ValidateWriter() != &w || g_traces != 0) { std::printf("00c31a70_TEST FAIL\n"); return 1; }
    p.m_pWriter = 0;
    if (p.ValidateWriter() != 0 || g_traces != 1 || !g_logged.empty()) { std::printf("00c31a70_TEST FAIL\n"); return 1; }
    g_pfnLogSink = LogSink;
    if (p.ValidateWriter() != 0 || g_traces != 2 || g_logged != ".\\PKSerialiseCParser.cpp(17) : Null writer\n")
        { std::printf("00c31a70_TEST FAIL\n"); return 1; }
    std::printf("00c31a70_TEST PASS\n");
    return 0;
}
#include <cstdio>
static int g_base = 0, g_strings = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
class CCharString { public: const char* m_text; long m_length; CCharString(const char* t, long l = -1) : m_text(t), m_length(l) { ++g_strings; } };
class CDataOutputStream : public CBase { public: CCharString NewLine; CDataOutputStream() : CBase(), NewLine("\r\n") { } };
int main()
{
    CDataOutputStream s;
    if (g_base != 1 || g_strings != 1 || s.NewLine.m_length != -1 || s.NewLine.m_text[0] != '\r' || s.NewLine.m_text[1] != '\n') { std::printf("00993e00_TEST FAIL\n"); return 1; }
    std::printf("00993e00_TEST PASS\n"); return 0;
}
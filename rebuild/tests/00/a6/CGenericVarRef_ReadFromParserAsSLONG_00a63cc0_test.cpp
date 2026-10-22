#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_type = 3, g_value = 55; static bool g_read = true;
struct CCharString { void* m_pData; CCharString() { std::strcat(g_seq, "c"); m_pData = 0; } ~CCharString() { std::strcat(g_seq, "d"); } };
struct SParsedItem { long m_type; long m_value; CCharString m_text; long GetAsLong() { std::strcat(g_seq, "L"); return m_value; } };
struct CStringParser { bool ReadNextItem(SParsedItem& item) { std::strcat(g_seq, "R"); item.m_type = g_type; item.m_value = g_value; return g_read; } };
struct CGenericVarRef { long Type; long* m_pValue; bool ReadFromParserAsSLONG(CStringParser& parser) { SParsedItem item; if (parser.ReadNextItem(item) && item.m_type == 3) { *m_pValue = item.GetAsLong(); return true; } return false; } };

int main() {
    CGenericVarRef v; long target = 0; v.m_pValue = &target; CStringParser p; if (!v.ReadFromParserAsSLONG(p) || std::strcmp(g_seq, "cRLd") || target != 55) { std::printf("00a63cc0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_type = 2; if (v.ReadFromParserAsSLONG(p) || std::strcmp(g_seq, "cRd") || target != 55) { std::printf("00a63cc0_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_type = 3; g_read = false; if (v.ReadFromParserAsSLONG(p) || std::strcmp(g_seq, "cRd")) { std::printf("00a63cc0_TEST FAIL\n"); return 1; }
    std::printf("00a63cc0_TEST PASS\n"); return 0;
}
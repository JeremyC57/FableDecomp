#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_type = 3, g_value = 55; static bool g_read = true;
class CCharString { public: void* m_pData; CCharString() { std::strcat(g_seq, "c"); m_pData = 0; } ~CCharString() { std::strcat(g_seq, "d"); } };
struct SParsedItem { long m_type; long m_value; CCharString m_text; long GetAsLong() { std::strcat(g_seq, "L"); return m_value; } };
class CStringParser { public: bool ReadNextItem(SParsedItem& item) { std::strcat(g_seq, "R"); item.m_type = g_type; item.m_value = g_value; return g_read; } };
class CGenericVar { public: unsigned char _pad[0x10]; long m_value; bool ReadFromParserAsULONG(CStringParser& parser) { SParsedItem item; if (parser.ReadNextItem(item) && item.m_type == 3) { m_value = item.GetAsLong(); return true; } return false; } };

int main() {
    CGenericVar v; v.m_value = 0; CStringParser p; if (!v.ReadFromParserAsULONG(p) || std::strcmp(g_seq, "cRLd") || v.m_value != 55) { std::printf("00a64130_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_type = 2; if (v.ReadFromParserAsULONG(p) || std::strcmp(g_seq, "cRd") || v.m_value != 55) { std::printf("00a64130_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_type = 3; g_read = false; if (v.ReadFromParserAsULONG(p) || std::strcmp(g_seq, "cRd")) { std::printf("00a64130_TEST FAIL\n"); return 1; }
    std::printf("00a64130_TEST PASS\n"); return 0;
}
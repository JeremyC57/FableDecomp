#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_byte = -1; static float g_float = 0; struct CDataOutputStream { void WriteCBYTE(long v) { std::strcat(g_seq, "B"); g_byte = v; } void WriteFloat(float v) { std::strcat(g_seq, "F"); g_float = v; } };
struct CSizedValue { long m_0; long m_count; float m_value; void TransferBinaryOut(CDataOutputStream* stream) { stream->WriteCBYTE(m_count); stream->WriteFloat(m_value); } };

int main() {
    CSizedValue v; v.m_0 = 0; v.m_count = 3; v.m_value = 2.5f; CDataOutputStream s; v.TransferBinaryOut(&s); if (std::strcmp(g_seq, "BF") || g_byte != 3 || g_float != 2.5f) { std::printf("00814c10_TEST FAIL\n"); return 1; }
    std::printf("00814c10_TEST PASS\n"); return 0;
}

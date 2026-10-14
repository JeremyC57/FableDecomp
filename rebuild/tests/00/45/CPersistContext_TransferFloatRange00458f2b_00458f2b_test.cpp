#include <cstdio>
#include <cstring>
static int g_crc = 0; static float g_written[2]; static int g_nWritten = 0; static int g_read = 0; static float g_readerMin = -1, g_readerMax = -1;
struct CFloatRange { float m_min; float m_max; };
struct CDataOutputStream { void WriteFloat(float v) { g_written[g_nWritten++] = v; } };
struct CDataInputStream { int x; };
struct CReader { float m_min, m_max; CReader() : m_min(0.0f), m_max(0.0f) { } void TransferBinaryIn(CDataInputStream*, CFloatRange*) { ++g_read; g_readerMin = m_min; g_readerMax = m_max; } };
struct CPersistContext { unsigned char _pad0[0x18]; long m_mode; unsigned char _pad1[8]; CDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char* n) { if (n[0] == 0) ++g_crc; }
    void Transfer(CFloatRange* v) { CheckCRC(""); CReader reader; switch (m_mode) { case 3: { CDataOutputStream* s = m_pSaveStream; s->WriteFloat(v->m_max); s->WriteFloat(v->m_min); break; } case 2: reader.TransferBinaryIn(m_pLoadStream, v); break; } } };
int main()
{
    CDataOutputStream out; CDataInputStream in; CFloatRange r = { 1.5f, 7.25f }; CPersistContext c; c.m_pLoadStream = &in; c.m_pSaveStream = &out;
    c.m_mode = 3; c.Transfer(&r); if (g_crc != 1 || g_nWritten != 2 || g_written[0] != 7.25f || g_written[1] != 1.5f || g_read != 0) { std::printf("00458f2b_TEST FAIL\n"); return 1; }
    c.m_mode = 2; c.Transfer(&r); if (g_crc != 2 || g_read != 1 || g_readerMin != 0.0f || g_readerMax != 0.0f || g_nWritten != 2) { std::printf("00458f2b_TEST FAIL\n"); return 1; }
    c.m_mode = 1; c.Transfer(&r); if (g_crc != 3 || g_read != 1 || g_nWritten != 2) { std::printf("00458f2b_TEST FAIL\n"); return 1; }
    std::printf("00458f2b_TEST PASS\n"); return 0;
}
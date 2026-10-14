#include <cstdio>
struct SBytePair { long m_first; long m_second; };
static long g_w[2]; static int g_n = 0; static long g_readerA = 0, g_readerB = 0;
struct CDataOutputStream { void WriteCBYTE(long v) { g_w[g_n++] = v; } };
struct CDataInputStream { };
static CDataOutputStream g_out; static CDataInputStream g_in;
struct CPairReader_004e9021 { long m_a; long m_b; CPairReader_004e9021() : m_a(-1), m_b(-1) { }
    void Read(CDataInputStream*, SBytePair* v) { g_readerA = m_a; g_readerB = m_b; v->m_first = 100; v->m_second = 200; } };
struct CPersistContext { unsigned char _pad0[0x18]; long m_mode; unsigned char _pad1[8]; CDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char*) { }
    void TransferBytePair_004e8fcd(SBytePair* value) { CheckCRC(""); CPairReader_004e9021 reader; switch (m_mode) { case 3: { CDataOutputStream* s = m_pSaveStream; s->WriteCBYTE(value->m_first); s->WriteCBYTE(value->m_second); break; } case 2: reader.Read(m_pLoadStream, value); break; } } };
int main() {
    CPersistContext c; c.m_mode = 3; c.m_pSaveStream = &g_out; c.m_pLoadStream = &g_in; SBytePair v = { 7, 9 };
    c.TransferBytePair_004e8fcd(&v); if (g_w[0] != 7 || g_w[1] != 9 || g_n != 2) { std::printf("004e8fcd_TEST FAIL\n"); return 1; }
    c.m_mode = 2; c.TransferBytePair_004e8fcd(&v); if (v.m_first != 100 || v.m_second != 200 || g_readerA != -1 || g_readerB != -1 || g_n != 2) { std::printf("004e8fcd_TEST FAIL\n"); return 1; }
    std::printf("004e8fcd_TEST PASS\n"); return 0;
}

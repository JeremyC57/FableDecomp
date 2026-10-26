#include <cstdio>
class CRGBColour { public: unsigned long m_argb; };
static unsigned long g_written = 0; static int g_crc = 0;
class CDataOutputStream { public: void WriteColour(const CRGBColour& c) { g_written = c.m_argb; } };
class CDataInputStream { public: unsigned long m_next; CRGBColour& ReadColour(CRGBColour& c) { c.m_argb = m_next; return c; } };
static CDataOutputStream g_out; static CDataInputStream g_in;
class CPersistContext { public: unsigned char _pad0[0x18]; long m_mode; unsigned char _pad1[8]; CDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream;
    void CheckCRC(const char*) { ++g_crc; }
    void TransferColour_004595a3(CRGBColour* value) { CheckCRC(""); switch (m_mode) { case 3: m_pSaveStream->WriteColour(*value); break; case 2: { CRGBColour colour; *value = m_pLoadStream->ReadColour(colour); break; } } } };
int main() {
    CPersistContext c; c.m_mode = 3; c.m_pSaveStream = &g_out; c.m_pLoadStream = &g_in; CRGBColour v; v.m_argb = 0x11223344;
    c.TransferColour_004595a3(&v); if (g_written != 0x11223344 || g_crc != 1) { std::printf("004595a3_TEST FAIL\n"); return 1; }
    c.m_mode = 2; g_in.m_next = 0xaabbccdd; c.TransferColour_004595a3(&v); if (v.m_argb != 0xaabbccdd || g_crc != 2) { std::printf("004595a3_TEST FAIL\n"); return 1; }
    c.m_mode = 5; c.TransferColour_004595a3(&v); if (v.m_argb != 0xaabbccdd || g_crc != 3) { std::printf("004595a3_TEST FAIL\n"); return 1; }
    std::printf("004595a3_TEST PASS\n"); return 0;
}
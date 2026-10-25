// behaviour test for 0x00C62750 ~CPKDecompressedStream: release the decompressor, close the source, null both
#define ADDR "00c62750"
#include <cstdio>
static int g_released = 0, g_closed = 0;
struct CDecompressorInterface {
    int m_unit, m_rate; int m_lastPos; bool m_lastRel;
    int GetUnitSize() { return m_unit; }
    int GetRate() { return m_rate; }
    int Seek(int position, bool relative) { m_lastPos = position; m_lastRel = relative; return position + 1000; }
    void Release() { ++g_released; }
};
struct CPKSourceObject { void Close() { ++g_closed; } };
struct CPKDecompressedStream {
    CPKSourceObject* m_pSource;
    CDecompressorInterface* m_pDecompressor;
    int m_position, m_userData;
    bool m_bFlag;
    int Seek(int mode, float amount, int userData)
    {
        if (!m_pDecompressor) return -1;
        bool relative = false;
        int position;
        switch (mode) {
        case 1: relative = true;
        case 2: position = (int)amount; break;
        case 3: relative = true;
        case 4: position = (int)(m_pDecompressor->GetUnitSize() * amount); break;
        case 5: relative = true;
        default: position = (int)(m_pDecompressor->GetRate() * amount * 0.001f); break;
        }
        m_position = m_pDecompressor->Seek(position, relative);
        m_userData = userData;
        m_bFlag = false;
        return m_position;
    }
    void Destroy()
    {
        if (m_pDecompressor) { m_pDecompressor->Release(); m_pDecompressor = 0; }
        if (m_pSource) { m_pSource->Close(); m_pSource = 0; }
    }
};
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL line %d\n", ADDR, __LINE__); return 1; } } while (0)
int main()
{
    CDecompressorInterface dec = { 4, 22050, 0, false };
    CPKSourceObject src;
    CPKDecompressedStream s = { &src, &dec, 0, 0, false };
    s.Destroy();
    CHECK(g_released == 1 && g_closed == 1 && !s.m_pSource && !s.m_pDecompressor);
    s.Destroy();
    CHECK(g_released == 1 && g_closed == 1);                         // both already null
    CPKDecompressedStream t = { 0, &dec, 0, 0, false };
    t.Destroy();
    CHECK(g_released == 2 && g_closed == 1);
    std::printf("00c62750_TEST PASS\n");
    return 0;
}
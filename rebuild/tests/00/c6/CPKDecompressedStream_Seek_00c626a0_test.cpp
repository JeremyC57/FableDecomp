// behaviour test for 0x00C626A0 CPKDecompressedStream::Seek: three unit modes, odd mode of each pair sets the flag
#define ADDR "00c626a0"
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
    CPKDecompressedStream s = { &src, 0, 0, 0, true };
    CHECK(s.Seek(1, 5.0f, 9) == -1 && s.m_bFlag);                     // no decompressor: untouched
    s.m_pDecompressor = &dec;
    CHECK(s.Seek(1, 10.7f, 9) == 1010 && dec.m_lastPos == 10 && dec.m_lastRel);
    CHECK(s.m_position == 1010 && s.m_userData == 9 && !s.m_bFlag);
    CHECK(s.Seek(2, 10.0f, 3) == 1010 && !dec.m_lastRel && s.m_userData == 3);
    CHECK(s.Seek(3, 2.5f, 0) == 1010 && dec.m_lastPos == 10 && dec.m_lastRel);
    CHECK(s.Seek(4, 3.0f, 0) == 1012 && !dec.m_lastRel);
    CHECK(s.Seek(5, 1000.0f, 0) == 23050 && dec.m_lastPos == 22050 && dec.m_lastRel);
    CHECK(s.Seek(6, 500.0f, 0) == 12025 && dec.m_lastPos == 11025 && !dec.m_lastRel);
    CHECK(s.Seek(0, 2000.0f, 0) == 45100 && !dec.m_lastRel);         // below the table: default
    std::printf("00c626a0_TEST PASS\n");
    return 0;
}
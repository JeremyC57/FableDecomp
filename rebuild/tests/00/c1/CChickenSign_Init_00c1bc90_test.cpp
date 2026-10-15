// Behaviour test for 0x00C1BC90 CChickenSign::Init: every packed field reset through its setter.
#include <cstdio>
long g_ftol(float v) { return (long)v; }
float g_defaultRangeMin = 1.0f, g_defaultRangeMax = 30.0f;
static long EncodeFixed16(float value) { return (long)(value * 65535.0f * 0.0001f); }
struct CChickenSign {
    unsigned char m_level, m_offset, m_flags, m_byteD;
    short m_wordB, m_wordC, m_scale;
    unsigned short m_wordA, m_wordE, m_time, m_rangeMin, m_rangeMax;
    void SetLevel(float v) { m_level = (unsigned char)(long)(v * 127.0f); }
    void SetScale(float v) { m_scale = (short)(long)(v * 100.0f); }
    void SetOffset(float v) { m_offset = (unsigned char)(long)(v * 100.0f); }
    void SetRangeMin(float v) { m_rangeMin = (unsigned short)EncodeFixed16(v); }
    void SetRangeMax(float v) { m_rangeMax = (unsigned short)EncodeFixed16(v); }
    void SetWordA(unsigned short v) { m_wordA = v; }
    void SetWordB(short v) { m_wordB = v; }
    void SetWordC(short v) { m_wordC = v; }
    void SetByteD(unsigned char v) { m_byteD = v; }
    void SetWordE(unsigned short v) { m_wordE = v; }
    void SetFlag0(bool on) { if (on) m_flags |= 1; else m_flags &= ~1; }
    void SetTime(float v) { m_time = (unsigned short)(long)(v * 65535.0f); }
    void Init()
    {
        m_flags = 0; SetLevel(1.0f); SetScale(1.0f); SetOffset(0.0f); SetRangeMin(g_defaultRangeMin); SetRangeMax(g_defaultRangeMax);
        SetWordA(1); SetWordB(0); SetWordC(0); SetByteD(0); SetWordE(0); SetFlag0(true); SetTime(1.0f);
    }
};
int main()
{
    CChickenSign s;
    s.m_flags = 0xff; s.m_wordB = 9; s.m_byteD = 7; s.m_wordE = 5;
    s.Init();
    if (s.m_level != 127 || s.m_scale != 100 || s.m_offset != 0 || s.m_rangeMin != 6 || s.m_rangeMax != 196 ||
        s.m_wordA != 1 || s.m_wordB != 0 || s.m_wordC != 0 || s.m_byteD != 0 || s.m_wordE != 0 || s.m_flags != 1 ||
        s.m_time != 65535) { std::printf("00c1bc90_TEST FAIL\n"); return 1; }
    std::printf("00c1bc90_TEST PASS\n");
    return 0;
}
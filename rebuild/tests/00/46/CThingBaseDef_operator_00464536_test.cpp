// Behaviour test for CThingBaseDef::operator= @ 0x00464536: base assignment then the derived dword, two 14-byte records, dword.
#include <cstdio>
struct SDefRecord14 { unsigned short m_values[7]; };
struct CThingBaseDefBase {
    long m_base; int baseAssigns;
    CThingBaseDefBase& operator=(const CThingBaseDefBase& other) { m_base = other.m_base; ++baseAssigns; return *this; }
};
class CThingBaseDef : public CThingBaseDefBase { public:
    long m_value70; SDefRecord14 m_record74; SDefRecord14 m_record82; long m_value90;
    CThingBaseDef& operator=(const CThingBaseDef& other)
    {
        CThingBaseDefBase::operator=(other);
        m_value70 = other.m_value70;
        m_record74 = other.m_record74;
        m_record82 = other.m_record82;
        m_value90 = other.m_value90;
        return *this;
    }
};
int main()
{
    CThingBaseDef a, b;
    a.m_base = 1; a.baseAssigns = 0; a.m_value70 = 2; a.m_value90 = 3;
    for (int i = 0; i < 7; ++i) { a.m_record74.m_values[i] = (unsigned short)(i + 10); a.m_record82.m_values[i] = (unsigned short)(i + 20); }
    b.m_base = 0; b.baseAssigns = 0; b.m_value70 = 0; b.m_value90 = 0;
    for (int j = 0; j < 7; ++j) { b.m_record74.m_values[j] = 0; b.m_record82.m_values[j] = 0; }
    CThingBaseDef& r = (b = a);
    if (&r != &b || b.m_base != 1 || b.baseAssigns != 1 || b.m_value70 != 2 || b.m_value90 != 3 ||
        b.m_record74.m_values[6] != 16 || b.m_record82.m_values[0] != 20 || b.m_record82.m_values[6] != 26) {
        std::printf("00464536_TEST FAIL\n"); return 1;
    }
    std::printf("00464536_TEST PASS\n");
    return 0;
}
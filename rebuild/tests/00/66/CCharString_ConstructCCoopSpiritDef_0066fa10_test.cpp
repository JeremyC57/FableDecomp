// Behaviour test for 0x0066FA10 CCoopSpiritDef::StatGetTypeName.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    CCharString(const char* text, int length)
    {
        int n = length < 0 ? (int)std::strlen(text) : length;
        if (n > 63) n = 63;
        std::memcpy(m_data, text, n); m_data[n] = 0;
    }
    const char* c_str() const { return m_data; }
private:
    char m_data[64];
};
class CCoopSpiritDef {
public:
    static CCharString StatGetTypeName() { return CCharString("CCoopSpiritDef", -1); }
};
int main()
{
    CCharString name = CCoopSpiritDef::StatGetTypeName();
    if (std::strcmp(name.c_str(), "CCoopSpiritDef") != 0) { std::printf("0066fa10_TEST FAIL\n"); return 1; }
    std::printf("0066fa10_TEST PASS\n");
    return 0;
}

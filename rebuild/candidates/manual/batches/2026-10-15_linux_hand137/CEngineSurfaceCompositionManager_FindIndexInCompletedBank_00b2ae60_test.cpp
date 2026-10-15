// Behaviour test for 0x00B2AE60 CEngineSurfaceCompositionManager::FindIndexInCompletedBank: lookup key is "<from>-<to>".
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[64];
    CCharString() { text[0] = 0; }
};
static CCharString GFIntToCharString(long value) { CCharString s; std::sprintf(s.text, "%ld", value); return s; }
static CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s; std::strcpy(s.text, l.text); std::strcat(s.text, r.text); return s; }
static CCharString operator+(const CCharString& l, const char* r) { CCharString s; std::strcpy(s.text, l.text); std::strcat(s.text, r); return s; }

struct CEngineTextureMorph { long m_fromTexture; long m_toTexture; };
static char g_key[64];
struct CGraphicDataBank {
    unsigned long FindIndex(const CCharString& name) { std::strcpy(g_key, name.text); return std::strcmp(name.text, "12-345") == 0 ? 7 : 0xffffffffu; }
};
struct CEngineSurfaceCompositionManager {
    CGraphicDataBank m_completedBank;
    unsigned long FindIndexInCompletedBank(const CEngineTextureMorph* morph)
    {
        long from = morph->m_fromTexture;
        long to = morph->m_toTexture;
        CCharString name = GFIntToCharString(from) + "-" + GFIntToCharString(to);
        return m_completedBank.FindIndex(name);
    }
};

int main()
{
    CEngineSurfaceCompositionManager m;
    CEngineTextureMorph morph; morph.m_fromTexture = 12; morph.m_toTexture = 345;
    if (m.FindIndexInCompletedBank(&morph) != 7 || std::strcmp(g_key, "12-345") != 0) { std::printf("00b2ae60_TEST FAIL\n"); return 1; }
    morph.m_toTexture = -1;
    if (m.FindIndexInCompletedBank(&morph) != 0xffffffffu || std::strcmp(g_key, "12--1") != 0) { std::printf("00b2ae60_TEST FAIL\n"); return 1; }
    std::printf("00b2ae60_TEST PASS\n");
    return 0;
}

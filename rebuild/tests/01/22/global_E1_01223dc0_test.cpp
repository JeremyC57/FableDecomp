// behaviour test for 0x01223DC0: the dynamic initializer of CSystemRegistry::GAME_REG_KEY (model with a counting string pair)
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString {
public:
    CCharString(const char* text, long length = -1) { std::strcpy(Text, text); (void)length; ++g_live; }
    ~CCharString() { --g_live; }
    char Text[64];
};
class CWideString {
public:
    CWideString() { Text[0] = 0; }
    static CWideString CreateFromCharString(const CCharString& text) { CWideString w; std::strcpy(w.Text, text.Text); return w; }
    char Text[64];
};
class CSystemRegistry { public: static const CWideString GAME_REG_KEY; };
const CWideString CSystemRegistry::GAME_REG_KEY = CWideString::CreateFromCharString("Software\\Microsoft\\Microsoft Games\\Fable TLC");
int main()
{
    int ok = std::strcmp(CSystemRegistry::GAME_REG_KEY.Text, "Software\\Microsoft\\Microsoft Games\\Fable TLC") == 0 && g_live == 0;
    if (!ok) { std::printf("01223dc0_TEST FAIL\n"); return 1; }
    std::printf("01223dc0_TEST PASS\n");
    return 0;
}
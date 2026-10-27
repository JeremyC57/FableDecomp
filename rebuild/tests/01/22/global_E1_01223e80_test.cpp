// behaviour test for 0x01223E80: the dynamic initializer of CSystemRegistry::PID_KEY_NAME (model with a counting string pair)
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
class CSystemRegistry { public: static const CWideString PID_KEY_NAME; };
const CWideString CSystemRegistry::PID_KEY_NAME = CWideString::CreateFromCharString("PID");
int main()
{
    int ok = std::strcmp(CSystemRegistry::PID_KEY_NAME.Text, "PID") == 0 && g_live == 0;
    if (!ok) { std::printf("01223e80_TEST FAIL\n"); return 1; }
    std::printf("01223e80_TEST PASS\n");
    return 0;
}
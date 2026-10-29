#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharString { public:
    char text[32];
    CCharString() { text[0] = 0; ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator+=(const CCharString& other) { std::strcat(text, other.text); return *this; }
};
CCharString GFIntToCharString(long value) { CCharString s; std::sprintf(s.text, "%ld", value); return s; }
void AppendLongToString(void* context, CCharString& text, const long& value)
{
    text += GFIntToCharString(value);
}
int main()
{
    {
        CCharString s; std::strcpy(s.text, "n=");
        long v = -42; AppendLongToString(0, s, v);
        long w = 7; AppendLongToString(&v, s, w);
        if (std::strcmp(s.text, "n=-427") || g_live != 1) { std::printf("009e2560_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("009e2560_TEST FAIL\n"); return 1; }
    std::printf("009e2560_TEST PASS\n");
    return 0;
}
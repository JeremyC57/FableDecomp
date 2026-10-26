#include <cstdio>
#include <cstring>
static int g_copies = 0;
class CWideString { public:
    char text[8];
    CWideString() { text[0] = 0; }
    CWideString(const CWideString& other) { std::strcpy(text, other.text); ++g_copies; }
    ~CWideString() {}
};
static CWideString g_EmptyWideString;
struct Inv {
    CWideString GetEmptyItemText(long itemType) { return g_EmptyWideString; }
};
int main()
{
    Inv inv;
    CWideString a = inv.GetEmptyItemText(5);
    CWideString b = inv.GetEmptyItemText(-1);
    if (a.text[0] != 0 || b.text[0] != 0 || g_copies < 2) { std::printf("004e7d4d_TEST FAIL\n"); return 1; }
    std::printf("004e7d4d_TEST PASS\n");
    return 0;
}
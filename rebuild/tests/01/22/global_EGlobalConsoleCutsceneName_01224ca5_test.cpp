#include <cstdio>
#include <cstring>
#include <cmath>
static int g_live = 0; static const char* g_text = 0; static int g_len = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { ++g_live; g_text = text; g_len = length; }
    ~CCharString() { --g_live; }
};

int main()
{
    {
        CCharString name("");
        if (g_live != 1 || g_text == 0 || g_text[0] != 0 || g_len != -1) do { std::printf("01224ca5_TEST FAIL\n"); return 1; } while (0);
    }
    if (g_live != 0) do { std::printf("01224ca5_TEST FAIL\n"); return 1; } while (0);
    std::printf("01224ca5_TEST PASS\n");
    return 0;
}
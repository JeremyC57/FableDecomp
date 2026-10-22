#include <cstdio>

struct CCharString {
    char* storage_;
    CCharString(const char* text, long length);
};

struct CTCMapwho {
    int dummy;
    CCharString GetClassTypeName();
};

static const char* g_lastText;
static long g_lastLen;
CCharString::CCharString(const char* text, long length) {
    g_lastText = text;
    g_lastLen  = length;
    storage_ = (char*)text;
}

int main() {
    CTCMapwho obj;
    obj.dummy = 0;
    CCharString r = obj.GetClassTypeName();
    if (g_lastLen == -1 && g_lastText != 0 && r.storage_ == g_lastText) {
        printf("0071a7f0_TEST PASS\n");
    } else {
        printf("FAIL len=%ld text=%p\n", g_lastLen, (void*)g_lastText);
    }
    return 0;
}
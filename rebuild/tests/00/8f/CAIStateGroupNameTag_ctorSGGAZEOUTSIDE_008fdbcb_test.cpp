#include <cstdio>

static const char* g_text = 0; static long g_len = 0;
class CCharString { public: void* p; CCharString(const char* t, long l) { g_text = t; g_len = l; p = 0; } };
struct CAIStateGroupNameTag { CCharString Name; CAIStateGroupNameTag() : Name("SG_GAZE_OUTSIDE", -1) {} };
int main()
{
    CAIStateGroupNameTag tag;
    if (!g_text || g_text[0] != 'S' || g_len != -1) { std::printf("008fdbcb_TEST FAIL\n"); return 1; }
    std::printf("008fdbcb_TEST PASS\n");
    return 0;
}
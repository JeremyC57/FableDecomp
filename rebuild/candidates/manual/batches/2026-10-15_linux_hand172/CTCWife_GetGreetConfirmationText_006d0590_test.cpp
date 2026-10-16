#include <cstdio>
#include <cstring>
static long g_frame = 100;
struct CTCWife {
    long married; bool sex; bool willing;
    bool Get(const char*& text) const {
        if (married <= g_frame) { if (sex) { text = "TEXT_GUI_WIFE_GO_TO_BED"; return true; } }
        else if (willing) { text = "TEXT_GUI_WIFE_MARRY_ME"; return true; }
        return false;
    }
};
int main()
{
    const char* t = 0;
    CTCWife a = { 50, true, false }, b = { 500, false, true }, c = { 50, false, true }, d = { 500, true, false };
    if (!a.Get(t) || std::strcmp(t, "TEXT_GUI_WIFE_GO_TO_BED") || !b.Get(t) || std::strcmp(t, "TEXT_GUI_WIFE_MARRY_ME") || c.Get(t) || d.Get(t)) { std::printf("006d0590_TEST FAIL\n"); return 1; }
    std::printf("006d0590_TEST PASS\n");
    return 0;
}

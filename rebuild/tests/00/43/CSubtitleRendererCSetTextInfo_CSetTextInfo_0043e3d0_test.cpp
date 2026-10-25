// Behaviour test for 0043e3d0: CSubtitleRenderer::CSetTextInfo::~CSetTextInfo -- the four strings destroyed last member first
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CWideString {
public:
    int Id;
    explicit CWideString(int id) : Id(id) {}
    ~CWideString() { g_order[g_count++] = Id; }
};
class CSubtitleRenderer {
public:
    class CSetTextInfo {
    public:
        CWideString Text, Button1Text, Button2Text, Button3Text;
        unsigned long DisplayFlags; float DisplayTime; bool UpdateWhilePaused; bool ChangePlayerMode;
        CSetTextInfo() : Text(0), Button1Text(1), Button2Text(2), Button3Text(3), DisplayFlags(5), DisplayTime(1.0f),
                         UpdateWhilePaused(true), ChangePlayerMode(false) {}
        ~CSetTextInfo() {}
    };
};
int main()
{
    { CSubtitleRenderer::CSetTextInfo info; }
    int ok = g_count == 4 && g_order[0] == 3 && g_order[1] == 2 && g_order[2] == 1 && g_order[3] == 0;
    std::printf(ok ? "0043e3d0_TEST PASS\n" : "0043e3d0_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for CFrontEndManager::~CFrontEndManager (0x005954AD): a model of the row.
#include <stdio.h>

static int g_deleted, g_keyboard, g_uiDestroyed; static void* g_pFrontEnd;
struct Screen { ~Screen() { g_deleted++; } };
struct FrontEnd {
    Screen* screens[3]; int n;
    ~FrontEnd() { g_keyboard++; for (int i = 0; i < n; ++i) delete screens[i]; g_uiDestroyed++; g_pFrontEnd = 0; }
};
static int run()
{
    FrontEnd* f = new FrontEnd; f->n = 3; for (int i = 0; i < 3; ++i) f->screens[i] = new Screen;
    g_pFrontEnd = f; delete f;
    return g_deleted == 3 && g_keyboard == 1 && g_uiDestroyed == 1 && g_pFrontEnd == 0;
}
int main()
{
    int ok = run();
    printf(ok ? "005954ad_TEST PASS\n" : "005954ad_TEST FAIL\n");
    return ok ? 0 : 1;
}

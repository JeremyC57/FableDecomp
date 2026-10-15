// Behaviour test for 0x0041E641 NUISystem::CManager::~CManager: error message cleared only when showing, both banks reset to empty, members then base destroyed.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
struct Bank { int* obj; Bank() : obj(0) { } };
struct FrontEnd { Bank font, graphics; void SetFontBank(Bank b) { font = b; g_log[g_n++] = 2; } void SetGraphicsBank(Bank b) { graphics = b; g_log[g_n++] = 3; } };
static FrontEnd g_fe;
static FrontEnd* GetInstance() { return &g_fe; }
struct Member { int id; ~Member() { g_log[g_n++] = id; } };
struct Base { virtual ~Base() { g_log[g_n++] = 99; } };
struct CManager : Base {
    Member a, b; bool showing;
    CManager(bool s) : showing(s) { a.id = 10; b.id = 11; }
    void ClearErrorMessage() { g_log[g_n++] = 1; showing = false; }
    virtual ~CManager()
    {
        if (showing) ClearErrorMessage();
        GetInstance()->SetFontBank(Bank());
        GetInstance()->SetGraphicsBank(Bank());
    }
};
int main()
{
    static int x;
    g_fe.font.obj = &x; g_fe.graphics.obj = &x;
    { CManager m(true); }
    bool ok = g_n == 6 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[3] == 11 && g_log[4] == 10 && g_log[5] == 99 &&
              g_fe.font.obj == 0 && g_fe.graphics.obj == 0;
    g_n = 0;
    { CManager m(false); }
    ok = ok && g_n == 5 && g_log[0] == 2;
    if (!ok) { std::printf("0041e641_TEST FAIL\n"); return 1; }
    std::printf("0041e641_TEST PASS\n");
    return 0;
}
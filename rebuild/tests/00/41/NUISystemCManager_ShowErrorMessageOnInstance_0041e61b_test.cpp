#include <stdio.h>
static int g_gets, g_shows; static long g_msg; static const void* g_this;
namespace NUISystem {
class CManager {
public:
    int dummy;
    static CManager* GetInstance();
    void ShowErrorMessage(long message) { ++g_shows; g_msg = message; g_this = this; }
    static void __fastcall ShowErrorMessageOnInstance(long message);
};
static CManager g_inst;
CManager* CManager::GetInstance() { ++g_gets; return &g_inst; }

void __fastcall CManager::ShowErrorMessageOnInstance(long message)
{
    GetInstance()->ShowErrorMessage(message);
}
}
int main()
{
    NUISystem::CManager::ShowErrorMessageOnInstance(0);
    bool ok = g_gets == 1 && g_shows == 1 && g_msg == 0 && g_this == &NUISystem::g_inst;
    NUISystem::CManager::ShowErrorMessageOnInstance(42);
    ok = ok && g_gets == 2 && g_msg == 42;
    printf(ok ? "0041e61b_TEST PASS\n" : "0041e61b_TEST FAIL\n");
    return ok ? 0 : 1;
}
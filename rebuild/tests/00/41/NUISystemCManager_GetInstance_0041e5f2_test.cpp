#include <stdio.h>
static int g_ctors;
namespace NUISystem {
class CManager {
public:
    unsigned char _body[0xd0];
    CManager() { ++g_ctors; _body[0] = 0x5a; }
    static CManager* GetInstance();
};
CManager* g_pUIManagerInstance;

CManager* CManager::GetInstance()
{
    if (g_pUIManagerInstance == 0)
        g_pUIManagerInstance = new CManager;
    return g_pUIManagerInstance;
}
}
int main()
{
    using namespace NUISystem;
    CManager* a = CManager::GetInstance();
    CManager* b = CManager::GetInstance();
    bool ok = a != 0 && a == b && g_ctors == 1 && a->_body[0] == 0x5a && g_pUIManagerInstance == a;
    CManager pre; g_pUIManagerInstance = &pre;
    ok = ok && CManager::GetInstance() == &pre && g_ctors == 2;
    printf(ok ? "0041e5f2_TEST PASS\n" : "0041e5f2_TEST FAIL\n");
    return ok ? 0 : 1;
}
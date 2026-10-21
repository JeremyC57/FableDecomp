// test for 0042fb40 NUISystem::CManager::Clear
#include <stdio.h>
namespace NUISystem {
class CObservable {
public:
    virtual void _slot00();
};
class CManager : public CObservable {
public:
    static void __fastcall Clear();
    static CManager* Instance;                           // 0x013B8710
protected:
    ~CManager();                                         // 0x0041E641
};
void __fastcall CManager::Clear()
{
    delete Instance;
    Instance = 0;
}
}

static int g_dtors; static void* g_last;
void NUISystem::CObservable::_slot00() {}
NUISystem::CManager::~CManager() { ++g_dtors; g_last = this; }
NUISystem::CManager* NUISystem::CManager::Instance;
int main()
{
    NUISystem::CManager* m = new NUISystem::CManager;
    NUISystem::CManager::Instance = m;
    NUISystem::CManager::Clear();
    int ok = g_dtors == 1 && g_last == m && NUISystem::CManager::Instance == 0;
    NUISystem::CManager::Clear();
    ok &= g_dtors == 1 && NUISystem::CManager::Instance == 0;
    printf(ok ? "0042fb40_TEST PASS\n" : "0042fb40_TEST FAIL\n");
    return ok ? 0 : 1;
}
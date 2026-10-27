// Behaviour test for 00546c20 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_Removed = 0, g_BaseDtor = 0; static void* g_Last = 0;
struct CObserver { virtual ~CObserver() { } int Pad; };
struct CManager { void RemoveObserver(CObserver* o) { ++g_Removed; g_Last = o; } static CManager* GetInstance() { static CManager m; return &m; } };
struct CUIInterface { virtual ~CUIInterface() { } };
struct CChangingStateComponent : CUIInterface, CObserver { virtual ~CChangingStateComponent() { ++g_BaseDtor; } };
struct CZoomingComponent : CChangingStateComponent { virtual ~CZoomingComponent() { CManager::GetInstance()->RemoveObserver(this); } };
int main() {
    CZoomingComponent* z = new CZoomingComponent; CObserver* obs = z;
    CUIInterface* p = z; delete p;
    if (g_Removed != 1 || g_Last != (void*)obs || g_BaseDtor != 1) { std::printf("00546c20_TEST FAIL\n"); return 1; }
    { std::printf("00546c20_TEST PASS\n"); return 0; }
}
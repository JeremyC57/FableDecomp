#include <cstdio>
enum FRONTEND_SCREEN { FRONTEND_SCREEN_23 = 23 };
namespace NUISystem { class CComponent; }
class CVirtualKeyboard {
public:
    CVirtualKeyboard(NUISystem::CComponent* screen);    // 0x00851700
    long Initialize();                                   // 0x00851770
    unsigned char _body[0x10];
};
namespace std {
template <class K, class V> class map {
public:
    unsigned char _body[0xc];
    V& operator[](const K& key);                         // 0x0059B5D7 for FRONTEND_SCREEN -> CComponent*
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;       // +0x54
    CVirtualKeyboard*                               m_pVirtualKeyboard; // +0x60
    void GotoNextScreen(CComponent* screen, bool flag);                 // 0x00596763
    void LaunchVirtualKeyboard();
};
void CFrontEndManager::LaunchVirtualKeyboard()
{
    GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_23], false);
    m_pVirtualKeyboard = new CVirtualKeyboard(m_mapScreens[FRONTEND_SCREEN_23]);
    m_pVirtualKeyboard->Initialize();
}
}

static NUISystem::CComponent* g_screen = (NUISystem::CComponent*)0x4321; static int g_keys[4]; static int g_nk;
static NUISystem::CComponent* g_gotoScreen; static int g_gotoFlag = -1; static NUISystem::CComponent* g_kbScreen; static int g_inits;
template <class K, class V> V& std::map<K, V>::operator[](const K& key) { g_keys[g_nk++] = key; return *(V*)&g_screen; }
CVirtualKeyboard::CVirtualKeyboard(NUISystem::CComponent* s) { g_kbScreen = s; }
long CVirtualKeyboard::Initialize() { ++g_inits; return 0; }
void NUISystem::CFrontEndManager::GotoNextScreen(CComponent* s, bool f) { g_gotoScreen = s; g_gotoFlag = f; }
int main()
{
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)];
    NUISystem::CFrontEndManager& m = *(NUISystem::CFrontEndManager*)raw; m.m_pVirtualKeyboard = 0;
    m.LaunchVirtualKeyboard();
    bool ok = g_nk == 2 && g_keys[0] == 23 && g_keys[1] == 23 && g_gotoScreen == g_screen && g_gotoFlag == 0
           && m.m_pVirtualKeyboard != 0 && g_kbScreen == g_screen && g_inits == 1;
    std::printf(ok ? "00596917_TEST PASS\n" : "00596917_TEST FAIL\n");
    return ok ? 0 : 1;
}
#include <cstdio>
class CPlayerGui { public: int x; }; static CPlayerGui* g_gui = 0; static void* g_list = 0;
namespace NPlayerGui {
class CDrawAbilityButtons { public: virtual void v00() { } unsigned char _body[0x28 - 4]; CDrawAbilityButtons(CPlayerGui* g, void* l) { g_gui = g; g_list = l; } };
struct CDrawAbilityButtonsPC : CDrawAbilityButtons { long m_buttonCount; CDrawAbilityButtonsPC(CPlayerGui* g, void* l) : CDrawAbilityButtons(g, l), m_buttonCount(2) { } }; }
int main() { CPlayerGui gui; int list; NPlayerGui::CDrawAbilityButtonsPC d(&gui, &list); if (g_gui != &gui || g_list != &list || d.m_buttonCount != 2) { std::printf("006579b0_TEST FAIL\n"); return 1; } std::printf("006579b0_TEST PASS\n"); return 0; }
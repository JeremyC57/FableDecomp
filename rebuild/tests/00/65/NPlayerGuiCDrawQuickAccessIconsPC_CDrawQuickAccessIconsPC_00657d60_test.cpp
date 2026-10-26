#include <cstdio>
class CPlayerGui { public: int x; }; class CEnginePrimitiveHandle { public: int x; };
static CPlayerGui* g_gui = 0; static void* g_list = 0; static long g_key = -1;
namespace NPlayerGui {
struct CRetailHandleMap { unsigned char _tree[8]; CEnginePrimitiveHandle h; CEnginePrimitiveHandle* operator[](long key) { g_key = key; return &h; } };
class CDrawQuickAccessIcons { public: virtual void v00() { } unsigned char _body[0x94 - 4]; CDrawQuickAccessIcons(CPlayerGui* g, void* l) { g_gui = g; g_list = l; } };
struct CDrawQuickAccessIconsPC : CDrawQuickAccessIcons { CRetailHandleMap m_handles; CDrawQuickAccessIconsPC(CPlayerGui* g, void* l) : CDrawQuickAccessIcons(g, l) { m_handles[2]; } }; }
int main() { CPlayerGui gui; int list; NPlayerGui::CDrawQuickAccessIconsPC d(&gui, &list); if (g_gui != &gui || g_list != &list || g_key != 2) { std::printf("00657d60_TEST FAIL\n"); return 1; } std::printf("00657d60_TEST PASS\n"); return 0; }
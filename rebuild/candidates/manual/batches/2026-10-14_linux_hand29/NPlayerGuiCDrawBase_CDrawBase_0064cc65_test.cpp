#include <cstdio>
struct CPlayerManager { int x; }; struct CWorld { int x; }; struct CMainGameComponent { int x; }; struct CGraphicDataBank { int x; };
struct CPlayerGui { unsigned char _pad00[4]; CWorld* m_pWorld; CPlayerManager* m_pPlayerManager; unsigned char _pad0c[4]; CMainGameComponent* m_pComponent; CGraphicDataBank* m_pBank; };
namespace NPlayerGui {
struct CDrawBase;
struct CRetailDrawList { CDrawBase** m_pFirst; CDrawBase** m_pLast; CDrawBase** m_pEnd; CDrawBase* buf[4]; void push_back(CDrawBase* const& v) { *m_pLast++ = v; } };
struct CRetailHandleVector { void* m_pFirst; void* m_pLast; void* m_pEnd; };
struct CDrawBase { virtual void v00() { } CPlayerGui* PlayerGui; CPlayerManager* PlayerManager; CWorld* World; CMainGameComponent* Component; CGraphicDataBank* Bank; bool Drawing; CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, CRetailDrawList* list) : PlayerGui(gui), PlayerManager(gui->m_pPlayerManager), World(gui->m_pWorld), Component(gui->m_pComponent), Bank(gui->m_pBank), Drawing(false)
    { PrimitiveHandles.m_pFirst = 0; PrimitiveHandles.m_pLast = 0; PrimitiveHandles.m_pEnd = 0; list->push_back(this); } }; }
int main() { CPlayerManager pm; CWorld w; CMainGameComponent c; CGraphicDataBank b; CPlayerGui gui; gui.m_pWorld = &w; gui.m_pPlayerManager = &pm; gui.m_pComponent = &c; gui.m_pBank = &b;
    NPlayerGui::CRetailDrawList list; list.m_pFirst = list.buf; list.m_pLast = list.buf; NPlayerGui::CDrawBase d(&gui, &list);
    if (d.PlayerGui != &gui || d.PlayerManager != &pm || d.World != &w || d.Component != &c || d.Bank != &b || d.Drawing || d.PrimitiveHandles.m_pFirst || d.PrimitiveHandles.m_pEnd || list.m_pLast - list.m_pFirst != 1 || list.buf[0] != &d) { std::printf("0064cc65_TEST FAIL\n"); return 1; } std::printf("0064cc65_TEST PASS\n"); return 0; }

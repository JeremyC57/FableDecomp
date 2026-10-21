// NUISystem::CFrontEndManager::LaunchVirtualKeyboard() -- go to the keyboard screen, create the virtual keyboard on it and initialise it
// Leaf: ?LaunchVirtualKeyboard@CFrontEndManager@NUISystem@@QAEXXZ
// Retail: Fable.exe @ 0x00596917, 99 bytes, plain `ret`; Ego_r.pdb ?LaunchVirtualKeyboard@CFrontEndManager@NUISystem@@QAEXXZ.
// Body: GotoNextScreen(m_mapScreens[23], false) (0x00596763; map::operator[] 0x0059B5D7 on +0x54, the key a stack
// temporary, 23 kept in esi for both lookups); m_pVirtualKeyboard = new CVirtualKeyboard(m_mapScreens[23]) (operator
// new 0x00BFEA1A, 0x10 bytes, null-tested; Ego_r.pdb ??0CVirtualKeyboard@@QAE@PAVCComponent@NUISystem@@@Z at retail
// 0x00851700); then m_pVirtualKeyboard->Initialize() (0x00851770, Ego_r.pdb ?Initialize@CVirtualKeyboard@@QAEJXZ, result
// ignored). No EH frame.
// Layout: retail CFrontEndManager m_mapScreens +0x54 (as 0x005966F2), m_pVirtualKeyboard +0x60 (Ego_r.pdb +0x64).
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
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
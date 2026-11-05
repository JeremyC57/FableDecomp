// NUISystem::CFrontEndManager::GotoPreviousScreen(bool force) -- leave the current screen for the one below it on the stack (screens 20/26/27/7/23 special-cased) and play the back sound
// Grade: DIFFER(517v517) -- 14 diff lines: retail reads top() before m_mapScreens[20] (value spilled to [ebp-8]) and spills the later map references to the dead param slot [ebp+8]; the back-sound tail pushes the hidden def pointer before saving the second GetInstance
// Leaf: ?GotoPreviousScreen@CFrontEndManager@NUISystem@@QAEX_N@Z
// Flags: /O1 /Oy-
// Retail: Fable.exe (TLC, VC7.1) @ 0x00597BF2, 517 bytes, unaligned entry after GotoProfileMenu 0x00597B20 (ends 0x00597BF1 with leave/ret), next 0x00597DF7:
//   a size-optimised frame-pointer TU (push 7 / pop ebx, pop ecx after operator new, leave) -> /O1 /Oy- (the landed siblings' optimize("s") + ("y",off)).
// Callees: deque<CComponent*>::back 0x0059B039, map<FRONTEND_SCREEN,CComponent*>::operator[] 0x0059B5D7, CUserProfileManager::Get 0x0040D2A0,
//   CUserProfileManager::Load 0x0040D350, CNewFrontendGameComponent::ResetAttractModeTimer 0x0042DBEE (Ego_r.pdb; bound as RestartAttractModeTimer),
//   CFrontEndManager::CloseVirtualKeyboard(bool) 0x00594FA9 (Ego_r.pdb; landed as DestroyVirtualKeyboard), _Deque_iterator_base::_M_subtract 0x0059A8F3,
//   IsScreenOnStack 0x005959AB, GotoSpecificPreviousScreen 0x0059686D, GotoNextScreen 0x00596763, CComponent::ChangeState(unsigned long) (slot 48),
//   CManager::RemoveObserver (CObservable slot 5), deque::pop_back 0x0059AEE5, CVirtualKeyboard(CComponent*) 0x00851700, CVirtualKeyboard::Initialize 0x00851770,
//   CManager::GetMiscThingsDef (slot 42) and PlaySoundW(CCharString&, long) (slot 40) with CUIMiscThingsDef::SoundBack +0x19C.
// Next idea: a spelling where top() is read first without a named local (the local swaps esi/edi and frees [ebp-8] for the spills), e.g. a
//   by-value stack top accessor in a CScreenStack wrapper, or the original STLport stack<CComponent*> with its own deque instance.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
namespace std {
template <class T> struct _Deque_iterator_base {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    int _M_subtract(const _Deque_iterator_base& other) const;   // 0x0059A8F3 for CComponent*
};
template <class T, class A = allocator<T> > class deque {
public:
    _Deque_iterator_base<T> _M_start;                    // +0x00
    _Deque_iterator_base<T> _M_finish;                   // +0x10
    T**                     _M_map;
    unsigned int            _M_map_size;
    unsigned int size() const { return _M_finish._M_subtract(_M_start); }
    T& back();                                           // 0x0059B039
    void pop_back();                                     // 0x0059AEE5
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    unsigned int size() const { return c.size(); }
    T& top() { return c.back(); }
    void pop() { c.pop_back(); }
};
}
enum FRONTEND_SCREEN { FRONTEND_SCREEN_7 = 7, FRONTEND_SCREEN_20 = 20, FRONTEND_SCREEN_23 = 23, FRONTEND_SCREEN_26 = 26, FRONTEND_SCREEN_27 = 27 };
class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool UseNewProfileValues;                            // +0x0C
    void Load();                                         // 0x0040D350
    static CUserProfileManager& __fastcall Get();        // 0x0040D2A0
};
class CNewFrontendGameComponent {
public:
    void ResetAttractModeTimer();                        // 0x0042DBEE
    static CNewFrontendGameComponent* m_pNewFrontendGameComponent;   // 0x013B871C
};
class CVirtualKeyboard {
public:
    unsigned char _pad00[0x10];
    CVirtualKeyboard(NUISystem::CComponent* screen);     // 0x00851700
    long Initialize();                                   // 0x00851770
};
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                          _pad00[0x20];
    std::stack<CComponent*>                m_stackScreens;   // +0x20
    unsigned char                          _pad48[0x54 - 0x48];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;     // +0x54
    CVirtualKeyboard*                      m_pVirtualKeyboard; // +0x60
    unsigned char                          _pad64[0x98 - 0x64];
    CComponent*                            PScreenToDeactivate;   // +0x98
    CComponent*                            PScreenToActivate;     // +0x9c
    void CloseVirtualKeyboard(bool goBack);              // 0x00594FA9
    bool IsScreenOnStack(CComponent* screen) const;      // 0x005959AB
    void GotoNextScreen(CComponent* screen, bool flag);  // 0x00596763
    void GotoSpecificPreviousScreen(CComponent* screen); // 0x0059686D
    void GotoPreviousScreen(bool force);
};
void CFrontEndManager::GotoPreviousScreen(bool force)
{
    if (!(m_mapScreens[FRONTEND_SCREEN_20] != m_stackScreens.top())) {
        if (!force)
            return;
        GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_26], false);
    } else {
        CUserProfileManager::Get().Load();
        CNewFrontendGameComponent::m_pNewFrontendGameComponent->ResetAttractModeTimer();
        CloseVirtualKeyboard(false);
        if (m_stackScreens.size() > 1) {
            if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_27]) {
                if (IsScreenOnStack(m_mapScreens[FRONTEND_SCREEN_7]))
                    GotoSpecificPreviousScreen(m_mapScreens[FRONTEND_SCREEN_7]);
                else
                    GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_20], true);
            } else {
                CComponent* leaving = m_stackScreens.top();
                leaving->ChangeState(6);
                CManager::GetInstance()->RemoveObserver(leaving);
                m_stackScreens.pop();
                PScreenToActivate = m_stackScreens.top();
                PScreenToDeactivate = leaving;
                if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_23]) {
                    CUserProfileManager::Get().UseNewProfileValues = true;
                    m_pVirtualKeyboard = new CVirtualKeyboard(m_mapScreens[FRONTEND_SCREEN_23]);
                    m_pVirtualKeyboard->Initialize();
                }
            }
        } else if (force)
            GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_26], false);
    }
    CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
}
}

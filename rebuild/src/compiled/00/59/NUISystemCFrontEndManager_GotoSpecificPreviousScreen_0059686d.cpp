// NUISystem::CFrontEndManager::GotoSpecificPreviousScreen(CComponent* screen) -- leave the current screen and unwind the screen stack back to a given screen (pushing it when the stack runs out)
// Leaf: ?GotoSpecificPreviousScreen@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z
// Retail: Fable.exe @ 0x0059686D, 170 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0) then its 0x0040D350 (catalog label
// CUserProfileManager::Load, not verified); if m_stackScreens.size() > 1 (deque iterator difference 0x0059A8F3, out of
// line): leaving = top() (deque::back 0x0059B039); leaving->slot 48 (+0xc0)(6); NUISystem::CManager::GetInstance()
// (0x0041E5F2, the lazily-created UI manager, vtable 0x01230134) slot 5 (+0x14, a CObservable observer-list removal)
// with leaving's CObserver base (+4); pop() (deque::pop_back 0x0059AEE5); pop while the stack is non-empty and its top
// is not `screen`; if it ran empty push(screen) (deque::push_back 0x0059B61C); PScreenToDeactivate = leaving,
// PScreenToActivate = screen.
// Layout: retail CFrontEndManager m_stackScreens +0x20 (STLport deque: start iterator +0, finish iterator +0x10),
// PScreenToDeactivate +0x98 / PScreenToActivate +0x9c (Ego_r.pdb +0x2c / +0xac / +0xb0). CComponent RTTI bases:
// CUIInterface +0, CObserver +4, CActionParam +0x18.
// Opt: size TU (unaligned entry) with a frame pointer.
// Shape: the unwind loop keeps the remaining count in a local re-read after each pop (`count = size()`), and `screen` is
// copied to a local: that gives retail's duplicated size() call, `screen` held in ebx and `this` spilled to [ebp-4]
// (a `while (size() > 0)` condition cross-jumps the two size() calls, 9 bytes shorter).
// NF86 (round 7, q11112 / q11113): the local std::deque / stack (and map) models are replaced by stlport/fable_deque.h (NF79 / NF83 opt-ins
//   STACK_TOP_POP + STACK_SIZE_PUSH + DEQUE_PUSH_BACK_OUTOFLINE, as NF83's 0x0059672A, and the NF86 overlay opt-in FABLE_RETAIL_STLP_STACK_EMPTY):
//   size() now binds the STLport _M_subtract 0x0059A8F3 (was ??G?$_Deque_iterator@..), and back 0x0059B039 / pop_back 0x0059AEE5 /
//   push_back 0x0059B61C bind the deque<NUISystem::CComponent*, allocator<..> > spellings their rows define (were the allocator-less forms).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_STACK_EMPTY
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
class CUserProfileManager {
public:
    void Load();                                         // 0x0040D350 (catalog name)
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T* Get();                                     // 0x0040D2A0 for CUserProfileManager
};
namespace NUISystem {
class CObserver {
public:
    virtual void _o00();
};
class CUIInterface {
public:
    virtual void _u00();
};
class CComponent : public CUIInterface, public CObserver {
public:
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006();
    virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012();
    virtual void _v013(); virtual void _v014(); virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018();
    virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024();
    virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036();
    virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040(); virtual void _v041(); virtual void _v042();
    virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void OnScreenEvent(long event);              // slot 48 (+0xc0, name not recovered; CComponent's own is empty)
};
class CManager {
public:
    virtual void _m00(); virtual void _m01(); virtual void _m02(); virtual void _m03(); virtual void _m04();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2
};
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];
    std::stack<CComponent*> m_stackScreens;              // +0x20
    unsigned char           _pad48[0x98 - 0x48];
    CComponent*             PScreenToDeactivate;         // +0x98
    CComponent*             PScreenToActivate;           // +0x9c
    void GotoSpecificPreviousScreen(CComponent* screen);
};

void CFrontEndManager::GotoSpecificPreviousScreen(CComponent* screen)
{
    CUserProfileManager::Get().Load();
    if (m_stackScreens.size() > 1)
    {
        CComponent* leaving = m_stackScreens.top();
        leaving->OnScreenEvent(6);
        CManager::GetInstance()->RemoveObserver(leaving);
        m_stackScreens.pop();
        unsigned int count = m_stackScreens.size();
        CComponent* target = screen;
        while (count > 0)
        {
            if (m_stackScreens.top() == target)
                break;
            m_stackScreens.pop();
            count = m_stackScreens.size();
        }
        if (m_stackScreens.empty())
            m_stackScreens.push(screen);
        PScreenToDeactivate = leaving;
        PScreenToActivate = target;
    }
}
}
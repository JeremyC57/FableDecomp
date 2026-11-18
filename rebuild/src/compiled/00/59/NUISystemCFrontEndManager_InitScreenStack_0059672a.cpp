// NUISystem::CFrontEndManager::InitScreenStack(NUISystem::CComponent* screen) -- register the first screen as an observer, let it observe all events, push it on the screen stack
// Leaf: ?InitScreenStack@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z
// Retail: Fable.exe @ 0x0059672A, 55 bytes, `ret 4`; one caller (0x00599CDA); Ego_r.pdb
// ?InitScreenStack@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z. Catalog label FrontEnd_FrameUpdate is wrong.
// Body: NUISystem::CManager::GetInstance() (0x0041E5F2, singleton 0x013B8710, vtable 0x01230134 : CObservable) ->
// AddObserver(screen) (slot 2, 0x0055CA40: list search, then insert) with screen converted to its CObserver base (+4,
// null-checked: `neg / sbb / and`); screen->ObserveAllEvents() (CObserver slot 6, 0x0052D7B0, through the +4 base);
// then m_stackScreens.push(screen) (deque::push_back 0x0059B61C on +0x20, by reference).
// Layout: CComponent (RTTI 0x0124608C: CUIInterface +0, CObserver +4, CActionParam +0x18); retail CFrontEndManager
// m_stackScreens +0x20 (40-byte STLport deque) as modelled at 0x005966F2; no engine/ headers for these.
// Opt: size TU (unaligned entry). Shape: manager and component locals bound after the GetInstance call keep screen+4 in a
// register across AddObserver (retail esi reuse); the expression form recomputes it from the stack.
#pragma optimize("s", on)
// NF83 (round 7, q11026 binder): the local std::deque / std::stack models replaced by stlport/fable_deque.h (stack::push inline,
//   deque::push_back out of line: FABLE_RETAIL_STLP_STACK_TOP_POP + _STACK_SIZE_PUSH + _DEQUE_PUSH_BACK_OUTOFLINE, the NF79 0x00596763 set);
//   push_back now binds the allocator-form ?push_back@?$deque@PAVCComponent@NUISystem@@V?$allocator@...@@QAEXABQAV... (both PDBs).
#define FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#include "stlport/fable_deque.h"
namespace NUISystem {
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    virtual void _slot01(); virtual void _slot02(); virtual void _slot03(); virtual void _slot04(); virtual void _slot05();
    virtual void ObserveAllEvents();                     // slot 6 (+0x18)
};
class CComponent : public CUIInterface, public CObserver {
public:
    long _body[4];
};
class CObservable {
public:
    virtual ~CObservable();
    virtual void _slot01();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+8)
};
class CManager : public CObservable {
public:
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char           _pad00[0x20];
    std::stack<CComponent*> m_stackScreens;              // +0x20
    void InitScreenStack(CComponent* screen);
};
void CFrontEndManager::InitScreenStack(CComponent* screen)
{
    CManager* manager = CManager::GetInstance();
    CComponent* component = screen;
    manager->AddObserver(component);
    component->ObserveAllEvents();
    m_stackScreens.push(screen);
}
}
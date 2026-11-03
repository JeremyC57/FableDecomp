// NTCScriptedControl::CActionStopScripting::Update() -- when this is the only queued action, end the running front action and drop the whole queue; otherwise just finish
// Leaf: ?Update@CActionStopScripting@NTCScriptedControl@@UAEXXZ
// Naming wave 28 (family d5): CActionBase is the nested CTCScriptedControl::CActionBase (Ego_r.pdb / FableWin.pdb;
//   DECISIONS_W27 D5) and PActionList the STLport list<CCountedPointer<CTCScriptedControl::CActionBase> >, whose clear 0x00719280
//   is ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ (c695). Was NTCScriptedControl::CActionBase (an invented namespace)
//   with the CActionList / SActionNode stand-ins (?clear@CActionList@@QAEXXZ).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713660, 75 bytes; ends in a tail `jmp` to the list clear.
// FableWin.pdb: ?Update@CActionStopScripting@NTCScriptedControl@@UAEXXZ (CActionBase vtable slot 1).
// Callees: CActionBase virtual slot 5 (+0x14, the end/cleanup call, as the landed CTCScriptedControl::InterruptControl
//          @ 0x00713850), the action list's clear() @ 0x00719280 (manifest "remove_if<...CConvFilterToCP...>" is a byte-twin
//          propagation; called with only the list as `this`).
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; retail CTCScriptedControl PActionList (STLport list,
//          the 4-byte sentinel pointer) +0x14; list node {next, prev, CCountedPointer {Data +8, Info +0xc}}.
// Shape: STLport size() is the inline distance loop counted in esi; `size() > 1` exits to the Terminated store; the
//          GetCurrentAction() idiom (`empty() ? 0 : front().Data`) then the slot-5 call and the tail-called clear().
class CThing;
#include "engine/CCountedPointer.h"
namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
protected:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // the 4-byte sentinel pointer
public:
    void clear();                                  // 0x00719280 (_list.c, out of line)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = this->_M_node._M_data->_M_next; p != this->_M_node._M_data; p = p->_M_next)
            ++n;
        return n;
    }
    _Tp& front() { return ((_List_node<_Tp>*)this->_M_node._M_data->_M_next)->_M_data; }
    void clear() { _Base::clear(); }
};
}
class CTCScriptedControl { public:
    class CActionBase;                             // nested (Ego_r.pdb / FableWin.pdb CTCScriptedControl::CActionBase)
    virtual void v00();
    CThing*      ParentThing;                      // +0x04 (CTCBase)
    bool         ToBeRemoved;                      // +0x08
    bool         IsInGlobalUpdateTCs;              // +0x09
    void*        PScriptHandleImp[2];              // +0x0c CIntelligentPointer
    std::list<CCountedPointer<CActionBase> > PActionList;   // +0x14
    bool         Locked;                           // +0x18
    CActionBase* GetCurrentAction();
    long GetNoActions() { return PActionList.size(); }
    void ClearActions();
};

class CTCScriptedControl::CActionBase {           // vtable 0x01265504
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnCleanup();                      // slot 5 (+0x14)
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

namespace NTCScriptedControl {
class CActionStopScripting : public CTCScriptedControl::CActionBase {
public:
    virtual void Update();
};
}

inline CTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList.front().Data;
}

inline void CTCScriptedControl::ClearActions()
{
    CActionBase* action = GetCurrentAction();
    if (action && action->Activated)
        action->OnCleanup();
    PActionList.clear();
}

namespace NTCScriptedControl {
void CActionStopScripting::Update()
{
    if (PTCScript->GetNoActions() <= 1)
        PTCScript->ClearActions();
    else
        Terminated = true;
}
}
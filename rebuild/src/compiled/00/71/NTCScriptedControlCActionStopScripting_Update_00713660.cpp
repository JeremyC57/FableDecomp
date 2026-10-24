// NTCScriptedControl::CActionStopScripting::Update() -- when this is the only queued action, end the running front action and drop the whole queue; otherwise just finish
// Leaf: ?Update@CActionStopScripting@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713660, 75 bytes; ends in a tail `jmp` to the list clear.
// FableWin.pdb: ?Update@CActionStopScripting@NTCScriptedControl@@UAEXXZ (CActionBase vtable slot 1).
// Callees: CActionBase virtual slot 5 (+0x14, the end/cleanup call, as the landed CTCScriptedControl::InterruptControl
//          @ 0x00713850), the action list's clear() @ 0x00719280 (manifest "remove_if<...CConvFilterToCP...>" is a byte-twin
//          propagation; called with only the list as `this`).
// Layout: CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9}; retail CTCScriptedControl PActionList (STLport list,
//          the 4-byte sentinel pointer) +0x14; list node {next, prev, CCountedPointer {Data +8, Info +0xc}}.
// Shape: STLport size() is the inline distance loop counted in esi; `size() > 1` exits to the Terminated store; the
//          GetCurrentAction() idiom (`empty() ? 0 : front().Data`) then the slot-5 call and the tail-called clear().
struct CThing;
struct CCPPointerInfo;

namespace NTCScriptedControl {
class CActionBase;
}

struct SActionNode {
    SActionNode*                     _M_next;      // +0x00
    SActionNode*                     _M_prev;      // +0x04
    NTCScriptedControl::CActionBase* Data;         // +0x08
    CCPPointerInfo*                  Info;         // +0x0c
};
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SActionNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void clear();                                  // 0x00719280
};
struct CTCScriptedControl {
    virtual void v00();
    CThing*      ParentThing;                      // +0x04 (CTCBase)
    bool         ToBeRemoved;                      // +0x08
    bool         IsInGlobalUpdateTCs;              // +0x09
    void*        PScriptHandleImp[2];              // +0x0c CIntelligentPointer
    CActionList  PActionList;                      // +0x14
    bool         Locked;                           // +0x18
    NTCScriptedControl::CActionBase* GetCurrentAction();
    long GetNoActions() { return PActionList.size(); }
    void ClearActions();
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnCleanup();                      // slot 5 (+0x14)
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionStopScripting : public CActionBase {
public:
    virtual void Update();
};
}

inline NTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList._M_node->_M_next->Data;
}

inline void CTCScriptedControl::ClearActions()
{
    NTCScriptedControl::CActionBase* action = GetCurrentAction();
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
// CTCScriptedControl::CleanupAllActions() -- let the running front action clean up (virtual OnCleanup) when it has been activated, then drop every queued action
// Leaf: ?CleanupAllActions@CTCScriptedControl@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00713630, 40 bytes, thiscall, tail `jmp` to the list clear.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?CleanupAllActions@CTCScriptedControl@@QAEXXZ; landed callers already bind this spelling
//   (CScriptGameResourceObjectScriptedThing::ClearCommands 0x00904310 and 8 more call sites); OnKill 0x00713700 (vtable 0x012658E4
//   slot 14) expands this same body before its CTCBase::OnKill tail, and the Ego_r donor order puts CleanupAllActions just before OnKill.
// Callees: CActionBase virtual slot 5 OnCleanup (+0x14; CActionTalkBase's slot-5 override is the landed OnCleanup 0x00714E00),
//   STLport std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase>, allocator<..> >::clear() 0x00719280 (with only the list as
//   `this`; the landed binding "?clear@CActionList@@QAEXXZ" is an invented spelling -- NAME_FIXES.tsv).
// Layout: retail CTCScriptedControl (Ego_r.pdb members at STLport sizes): CTCBase (vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs)
//   0x0C, PScriptHandleImp (CIntelligentPointer, 8) +0x0C, PActionList (STLport list, the 4-byte node pointer) +0x14, Locked +0x18.
//   CActionBase (Ego_r.pdb, 0x0C): vptr, PTCScript +0x04, Activated +0x08, Terminated +0x09.
// Shape: STLport list empty() / front(), the CCountedPointer operator-> (engine/CCountedPointer.h) and CActionBase::IsActive().
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
protected:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
public:
    void clear();                                          // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& front() { return ((_List_node<_Tp>*)this->_M_node._M_data->_M_next)->_M_data; }
    void clear() { _Base::clear(); }
};
}
class CThing;
class CCharString;
class CTCBase : public CBaseClassNonCopyable {               // engine/CTCBase.h flattens the vptr: local polymorphic model
public:
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CScriptGameResourceObjectScriptedThing;
class CTCScriptedControl : public CTCBase {
public:
    class CActionBase : public CBaseClassNonCopyable {     // vtable 0x01265504
    public:
        virtual void Update() = 0;                         // slot 1 (pure; CActionWait::Update 0x00712240 = FableWin ?Update@CActionWait@..)
        virtual void OnActivate();                         // slot 2
        virtual void OnAdd();                              // slot 3
        virtual CCharString GetActionName() const = 0;     // slot 4 (pure; CActionWait 0x00903640 = ?GetActionName@CActionWait@..@@UBE?AVCCharString@@XZ)
        virtual void OnCleanup();                          // slot 5 (+0x14)
        bool IsActive() const { return Activated; }
        CTCScriptedControl* PTCScript;                     // +0x04
        bool Activated;                                    // +0x08
        bool Terminated;                                   // +0x09
    };
    CIntelligentPointer<CScriptGameResourceObjectScriptedThing> PScriptHandleImp;   // +0x0C
    std::list<CCountedPointer<CActionBase> > PActionList;                         // +0x14
    bool Locked;                                                                    // +0x18
    void CleanupAllActions();
};

void CTCScriptedControl::CleanupAllActions()
{
    if (!PActionList.empty()) {
        CActionBase* action = PActionList.front().Get();
        if (action && action->IsActive())
            action->OnCleanup();
    }
    PActionList.clear();
}
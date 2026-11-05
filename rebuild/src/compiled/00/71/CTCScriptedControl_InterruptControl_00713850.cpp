// CTCScriptedControl::InterruptControl() -- end the running front action (virtual slot 5) when it has started, drop every queued action, tell the quest manager the thing was interrupted, clear the script handle and unlock
// Leaf: InterruptControl
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 28 (family d5): CActionBase is the nested CTCScriptedControl::CActionBase (Ego_r.pdb / FableWin.pdb;
//   DECISIONS_W27 D5) and PActionList the STLport list<CCountedPointer<CTCScriptedControl::CActionBase> >, whose clear 0x00719280
//   is ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ (c695). Was a global struct CActionBase
//   with the CActionList / SActionNode stand-ins (?clear@CActionList@@QAEXXZ).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe 0x00713850, 125 bytes (plain `ret`).
// Callees: CActionBase virtual slot 5 (+0x14), the action list's clear() @ 0x00719280 (manifest "remove_if<...CConvFilterToCP...>"
// is a byte-twin propagation; called with only the list as `this`), CQuestManager::OnEntityInterrupted(CThing*)
// @ 0x004AFBB0 on the singleton @ 0x013B89FC, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
// GetPItem 0x00A01B50 / dtor 0x00A01C10; the temporary's vtable 0x0126552C = CIntelligentPointer (rtti-vtables).
// Layout: retail CTCScriptedControl: PScriptHandleImp (CIntelligentPointer, 8 bytes) +0x0C, PActionList (STLport list,
// the 4-byte sentinel pointer) +0x14, Locked +0x18 (PDB +0x20 with the 12-byte debug list). CActionBase: Activated +0x08.
// Shape: GetCurrentAction() = `empty() ? 0 : front().Data` inline; the handle reset is `p = CIntelligentPointer(0)`.
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CBaseObject* m_pItem;                           // +4
    CBaseIntelligentPointer();                      // 0x00A01B10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {    // CIntelligentPointer<CScriptGameResourceObjectScriptedThing>, vtable 0x0126552C
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    void OnEntityInterrupted(CThing* thing);        // 0x004AFBB0
};

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
    _Tp& front() { return ((_List_node<_Tp>*)this->_M_node._M_data->_M_next)->_M_data; }
    void clear() { _Base::clear(); }
};
}
class CTCScriptedControl { public:
    class CActionBase {                             // vtable 0x01265504 (Ego_r.pdb / FableWin.pdb CTCScriptedControl::CActionBase)
    public:
        virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
        virtual void Exit();                        // slot 5 (+0x14)
        void* PTCScript;                            // +0x04
        bool  Activated;                            // +0x08
        bool  Terminated;                           // +0x09
    };
    virtual void v00();
    CThing*             ParentThing;                // +0x04 (CTCBase)
    bool                ToBeRemoved;                // +0x08
    bool                IsInGlobalUpdateTCs;        // +0x09
    CIntelligentPointer PScriptHandleImp;           // +0x0c
    std::list<CCountedPointer<CActionBase> > PActionList;   // +0x14
    bool                Locked;                     // +0x18
    CActionBase* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList.front().Data; }
    void InterruptControl();
};

void CTCScriptedControl::InterruptControl()
{
    CActionBase* action = GetCurrentAction();
    if (action && action->Activated)
        action->Exit();
    PActionList.clear();
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().OnEntityInterrupted(ParentThing);
    PScriptHandleImp = CIntelligentPointer(0);
    Locked = false;
}
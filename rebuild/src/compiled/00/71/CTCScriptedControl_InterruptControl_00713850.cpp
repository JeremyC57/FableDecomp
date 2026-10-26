// CTCScriptedControl::InterruptControl() -- end the running front action (virtual slot 5) when it has started, drop every queued action, tell the quest manager the thing was interrupted, clear the script handle and unlock
// Leaf: InterruptControl
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
class CQuestManager { public:
    void OnEntityInterrupted(CThing* thing);        // 0x004AFBB0
};
extern CQuestManager* g_pQuestManager;              // 0x013B89FC

struct CActionBase {                                // CTCScriptedControl::CActionBase, vtable 0x01265504
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Exit();                            // slot 5 (+0x14)
    void* PTCScript;                                // +0x04
    bool  Activated;                                // +0x08
    bool  Terminated;                               // +0x09
};
class CCPPointerInfo;
struct SActionNode {
    SActionNode*    _M_next;                        // +0x00
    SActionNode*    _M_prev;                        // +0x04
    CActionBase*    Data;                           // +0x08
    CCPPointerInfo* Info;                           // +0x0c
};
struct CActionList {                                // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    void clear();                                   // 0x00719280
};
class CTCScriptedControl { public:
    virtual void v00();
    CThing*             ParentThing;                // +0x04 (CTCBase)
    bool                ToBeRemoved;                // +0x08
    bool                IsInGlobalUpdateTCs;        // +0x09
    CIntelligentPointer PScriptHandleImp;           // +0x0c
    CActionList         PActionList;                // +0x14
    bool                Locked;                     // +0x18
    CActionBase* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList._M_node->_M_next->Data; }
    void InterruptControl();
};

void CTCScriptedControl::InterruptControl()
{
    CActionBase* action = GetCurrentAction();
    if (action && action->Activated)
        action->Exit();
    PActionList.clear();
    g_pQuestManager->OnEntityInterrupted(ParentThing);
    PScriptHandleImp = CIntelligentPointer(0);
    Locked = false;
}
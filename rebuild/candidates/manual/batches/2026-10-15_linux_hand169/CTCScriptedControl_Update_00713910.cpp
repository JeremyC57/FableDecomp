// CTCScriptedControl::Update() -- drive the front action: start it if not yet activated (slot 2), update it while not terminated (slot 1), and once terminated end it (slot 5) and erase it from PActionList
// Leaf: Update
// Retail: Fable.exe 0x00713910, 109 bytes (plain `ret`).
// Callees: CActionBase virtual slots 2 (+0x08), 1 (+0x04) and 5 (+0x14); the action list's erase(iterator) @ 0x00719210
// (manifest "list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>>::erase" is a byte-twin propagation),
// iterator passed by value (built in its argument slot) and returned through the hidden pointer (the `push ecx` word).
// Layout: retail CTCScriptedControl: PActionList (STLport list, 4-byte sentinel pointer) +0x14; CActionBase:
// Activated +0x08, Terminated +0x09.
// Shape: each test re-reads the list head through the list pointer (esi); the end arm uses the inline
// GetCurrentAction() (empty ? 0 : front().Data), whose Data load is CSE'd with the Terminated test.
struct CActionBase {                                // CTCScriptedControl::CActionBase, vtable 0x01265504
    virtual void v00();
    virtual void Update();                          // slot 1 (+0x04)
    virtual void Start();                           // slot 2 (+0x08)
    virtual void v03(); virtual void v04();
    virtual void Exit();                            // slot 5 (+0x14)
    void* PTCScript;                                // +0x04
    bool  Activated;                                // +0x08
    bool  Terminated;                               // +0x09
};
struct CCPPointerInfo;
struct SActionNode {
    SActionNode*    _M_next;                        // +0x00
    SActionNode*    _M_prev;                        // +0x04
    CActionBase*    Data;                           // +0x08
    CCPPointerInfo* Info;                           // +0x0c
};
struct CActionList {                                // STLport list<CCountedPointer<CActionBase>>
    struct iterator {
        SActionNode* _M_node;
        iterator(SActionNode* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
    };
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CActionBase* front() const { return _M_node->_M_next->Data; }
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator erase(iterator pos);                   // 0x00719210
};
struct CTCScriptedControl {
    virtual void v00();
    void*         ParentThing;                      // +0x04 (CTCBase)
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    unsigned char PScriptHandleImp[8];              // +0x0c
    CActionList   PActionList;                      // +0x14
    bool          Locked;                           // +0x18
    CActionBase* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList.front(); }
    void Update();
};

void CTCScriptedControl::Update()
{
    if (!PActionList.empty() && !PActionList.front()->Activated)
        PActionList.front()->Start();
    if (!PActionList.empty() && !PActionList.front()->Terminated)
        PActionList.front()->Update();
    if (!PActionList.empty() && PActionList.front()->Terminated) {
        CActionBase* action = GetCurrentAction();
        if (action) {
            action->Exit();
            PActionList.erase(PActionList.begin());
        }
    }
}

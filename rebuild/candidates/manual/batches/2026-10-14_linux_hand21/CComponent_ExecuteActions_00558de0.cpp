// NUISystem::CComponent::ExecuteActions(std::list<CCountedPointer<CAction>>*) -- hand every action of the list to the front-end manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558DE0, length 45 bytes.
// Callees: NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, no arguments), then
// its vtable slot 14 (+0x38) ExecuteAction(const CCountedPointer<CAction>&) with the node's value
// (`lea ecx,[esi+8]`). Vtable 0x0124B4E0 (the CNavButton TU) points at it; 3 bytes of int3 padding follow.
// The list is the retail std::list (sentinel pointer only; node = {next, prev, value}) walked with the
// retail iterator object, so end() re-reads the head pointer from the list on every pass. A null list
// pointer is skipped. Slot 131 of the NUISystem component vtables is this body, emitted once per
// translation unit that instantiates a component (the same 45-byte body also sits @ 0x00536B22 /
// 0x00558DE0).
struct CAction;

struct CCountedActionPointer {                                  // CCountedPointer<NUISystem::CAction>
    CAction* m_pObject;
    void*    m_pCount;
};

template<class T> struct CRetailList {                          // retail std::list: the sentinel pointer only
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        explicit iterator(node* q) : p(q) { }
        T& operator*() const { return p->value; }
        iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
};

struct CFrontEndManager_Calls {                                // engine/CFrontEndManager.h holds data only: this is the vtable-slot view
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void ExecuteAction(const CCountedActionPointer& action);   // slot 14 (+0x38)
    static CFrontEndManager_Calls* __fastcall GetInstance();          // 0x0041E5F2
};

struct CComponent {
    virtual void v00();
    void ExecuteActions(CRetailList<CCountedActionPointer>* actions);
};

void CComponent::ExecuteActions(CRetailList<CCountedActionPointer>* actions)
{
    if (actions != 0) {
        for (CRetailList<CCountedActionPointer>::iterator it = actions->begin(); it != actions->end(); ++it)
            CFrontEndManager_Calls::GetInstance()->ExecuteAction(*it);
    }
}

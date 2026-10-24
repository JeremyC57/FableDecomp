// NUISystem::CComponent::ExecuteActions(std::list<CCountedPointer<CAction>>*) -- hand every action of the list to the front-end manager
// Leaf: ExecuteActions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005309B0, length 43 bytes (+ int3 padding).
// Callees: CFrontEndManager::GetInstance @ 0x0041E5F2 (lazy singleton), its vslot 14 (`call [edx+38h]`)
// with the node's counted pointer (+8) by reference.
// Vtable 0x0122F5D4 slot 131. `this` is unused. The list is the retail {head, size} Dinkumware list whose
// nodes are {next, prev, value}; the head is re-read from the list after each call, the first compare uses
// the freshly loaded head.
struct CAction;
struct CCountedPointer_CAction { CAction* m_pObject; void* m_pCount; };

struct CActionListNode {
    CActionListNode*       next;               // +0
    CActionListNode*       prev;               // +4
    CCountedPointer_CAction value;             // +8
};

struct CActionList {                           // std::list<CCountedPointer<NUISystem::CAction>>
    CActionListNode* head;                     // +0 the sentinel
    unsigned long    size;                     // +4
};

struct CFrontEndManager_Calls {               // engine/CFrontEndManager.h exists; only vslot 14 and the singleton getter are needed
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer_CAction& action);   // slot 14
    static CFrontEndManager_Calls* GetInstance();   // 0x0041E5F2
};

struct CComponent_Retail {
    virtual void v00();
    void ExecuteActions(CActionList* actions);
};

void CComponent_Retail::ExecuteActions(CActionList* actions)
{
    if (actions) {
        for (CActionListNode* node = actions->head->next; node != actions->head; node = node->next)
            CFrontEndManager_Calls::GetInstance()->ExecuteAction(node->value);
    }
}
// CAction::~CAction() -- delete every owned element of the first list (ownership word != 1), then destroy both lists
// Leaf: ??1CAction
// (second first): each frees its nodes, re-links the head to itself and frees the head.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053FD00, length 164 bytes (+ int3 padding).
// Callees: the element's virtual slot 0 (scalar deleting destructor, flag 1) and operator delete @ 0x00BFEA14
// (the nodes and the two heads).
// Shape: two head-only linked lists at +4 and +8 (node: next +0, prev +4, value +8) with inline clear/destructor
// (the head re-read through `this` on every step; nodes freed before the head is re-linked); `delete p` is the
// null test + virtual deleting destructor, with the element re-read from the node (a cached local lets VC drop
// delete's null test after the ownership read). No vtable store, so CAction is modelled without virtuals; the word at +0
// is unknown. No engine header and no landed relatives; local model.
void __cdecl free(void*);   // 0x00BFEA14 (operator delete)

template<class T> struct CActionList {          // head-only doubly linked list
    struct SNode {
        SNode* next;                            // +0x00
        SNode* prev;                            // +0x04
        T value;                                // +0x08
    };
    SNode* head;
    void clear()
    {
        SNode* node = head->next;
        while (node != head) {
            SNode* dead = node;
            node = node->next;
            free(dead);
        }
        head->next = head;
        head->prev = head;
    }
    ~CActionList()
    {
        clear();
        if (head)
            free(head);
    }
};

struct CActionElement {
    virtual ~CActionElement();                  // slot 0: scalar deleting destructor
    long m_ownership;                           // +0x04 (1 = not owned by the action)
};

struct CAction {
    long m_unknown0;                            // +0x00
    CActionList<CActionElement*> m_elements;    // +0x04
    CActionList<void*> m_pending;               // +0x08
    ~CAction();
};

CAction::~CAction()
{
    for (CActionList<CActionElement*>::SNode* node = m_elements.head->next; node != m_elements.head; node = node->next) {
        if (node->value->m_ownership != 1)
            delete node->value;
    }
}
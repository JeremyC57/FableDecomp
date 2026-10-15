#include <cstdio>
// Behaviour test for 00647357.
// Quest-info test model: a circular sentinel list of counted element pointers, polymorphic elements with an ID,
// a script ID, a scale and a type; the drawer keeps the element list plus the mini-game index.
struct CQuestInfoElement {
    long ID;
    long ScriptID;
    float Scale;
    int cleared;
    CQuestInfoElement() : ID(0), ScriptID(0), Scale(1.0f), cleared(0) { }
    virtual ~CQuestInfoElement() { }
    virtual void Clear() { ++cleared; }
    virtual long GetType() { return 0; }
};
struct CCountedElementPointer { CQuestInfoElement* m_pObject; void* m_pBlock; };
struct CRetailElementList {
    struct node { node* next; node* prev; CCountedElementPointer value; };
    node sentinel;
    node pool[16];
    int used;
    node* head;
    CRetailElementList() : used(0), head(&sentinel) { sentinel.next = sentinel.prev = &sentinel; }
    void push_back(CQuestInfoElement* e)
    {
        node* n = &pool[used++];
        n->value.m_pObject = e; n->value.m_pBlock = 0;
        n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n;
    }
    bool empty() const { return head->next == head; }
    node* erase(node* n) { node* next = n->next; n->prev->next = next; next->prev = n->prev; return next; }
    int size() const { int c = 0; for (node* n = head->next; n != head; n = n->next) ++c; return c; }
};

// NPlayerGui::CDrawQuestInfo::RemoveElement: the first element with the id is cleared and erased, the box hides when the
// list becomes empty, and the drawer's primitive handles are all cleared.
struct CHandle { int cleared; void Clear() { ++cleared; } };
struct CDrawQuestInfo {
    CHandle handles[3]; int handleCount;
    CRetailElementList Elements;
    int hidden;
    CDrawQuestInfo() : handleCount(3), hidden(0) { for (int i = 0; i < 3; ++i) handles[i].cleared = 0; }
    void DisplayQuestInfo(bool display) { if (!display) ++hidden; }
    void RemoveElement(long id)
    {
        CRetailElementList* list = &Elements;
        for (CRetailElementList::node* n = list->head->next; n != list->head; n = n->next) {
            CQuestInfoElement* element = n->value.m_pObject;
            if (element->ID == id) { element->Clear(); list->erase(n); break; }
        }
        if (list->empty()) DisplayQuestInfo(false);
        for (int i = 0; i < handleCount; ++i) handles[i].Clear();
    }
};
int main()
{
    CQuestInfoElement a, b; a.ID = 1; b.ID = 2;
    CDrawQuestInfo q; q.Elements.push_back(&a); q.Elements.push_back(&b);
    int ok = 1;
    q.RemoveElement(2);
    if (b.cleared != 1 || a.cleared != 0 || q.Elements.size() != 1 || q.hidden != 0 || q.handles[2].cleared != 1) ok = 0;
    q.RemoveElement(7);
    if (q.Elements.size() != 1 || q.hidden != 0) ok = 0;
    q.RemoveElement(1);
    if (a.cleared != 1 || !q.Elements.empty() || q.hidden != 1 || q.handles[0].cleared != 3) ok = 0;
    if (!ok) { std::printf("00647357_TEST FAIL\n"); return 1; }
    std::printf("00647357_TEST PASS\n");
    return 0;
}

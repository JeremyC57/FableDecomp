#include <cstdio>
// Behaviour test for 00654593.
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

// NPlayerGui::CDrawQuestInfo::DisplayMiniGameInfo: removes the current mini-game bar's extra elements and the bar, then adds a new one when asked.
struct CMiniGameBar : CQuestInfoElement { long extra[4]; unsigned int extraCount; CMiniGameBar() : extraCount(0) { } long GetType() { return 6; } };
struct CDrawQuestInfo {
    CRetailElementList Elements; long MiniGameElementIndex;
    long removed[8]; int removedCount; int added;
    CDrawQuestInfo() : MiniGameElementIndex(0), removedCount(0), added(0) { }
    void RemoveElement(long id) { removed[removedCount++] = id; }
    long AddMiniGameBar(long type, long graphic) { ++added; return type * 10 + graphic; }
    void DisplayMiniGameInfo(bool show, long type, long graphic)
    {
        if (MiniGameElementIndex > 0) {
            for (CRetailElementList::node* n = Elements.head->next; n != Elements.head; n = n->next) {
                CQuestInfoElement* element = n->value.m_pObject;
                if (element->ID == MiniGameElementIndex && element->GetType() == 6) {
                    CMiniGameBar* bar = static_cast<CMiniGameBar*>(element);
                    for (unsigned int i = 0; i < bar->extraCount; ++i) RemoveElement(bar->extra[i]);
                }
            }
            RemoveElement(MiniGameElementIndex);
            MiniGameElementIndex = 0;
        }
        if (show) MiniGameElementIndex = AddMiniGameBar(type, graphic);
    }
};
int main()
{
    CMiniGameBar bar; bar.ID = 5; bar.extra[0] = 6; bar.extra[1] = 7; bar.extraCount = 2;
    CDrawQuestInfo q; q.Elements.push_back(&bar);
    int ok = 1;
    q.DisplayMiniGameInfo(true, 2, 3);
    if (q.removedCount != 0 || q.MiniGameElementIndex != 23) ok = 0;
    q.MiniGameElementIndex = 5;
    q.DisplayMiniGameInfo(false, 0, 0);
    if (q.removedCount != 3 || q.removed[0] != 6 || q.removed[1] != 7 || q.removed[2] != 5 || q.MiniGameElementIndex != 0 || q.added != 1) ok = 0;
    if (!ok) { std::printf("00654593_TEST FAIL\n"); return 1; }
    std::printf("00654593_TEST PASS\n");
    return 0;
}

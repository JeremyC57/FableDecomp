#include <cstdio>
// Behaviour test for 00644d91.
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

// NPlayerGui::CDrawQuestInfo::UpdateBar: the bar with the id gets the value; max/min only when not negative; other types untouched.
struct CBar : CQuestInfoElement { float Value, MaxValue, MinValue; CBar() : Value(0), MaxValue(5), MinValue(1) { } long GetType() { return 1; } };
struct COther : CQuestInfoElement { long GetType() { return 3; } };
struct CDrawQuestInfo {
    CRetailElementList Elements;
    void UpdateBar(long id, float value, float maxValue, float minValue)
    {
        for (CRetailElementList::node* n = Elements.head->next; n != Elements.head; n = n->next) {
            CQuestInfoElement* element = n->value.m_pObject;
            if (element != 0 && element->ID == id) {
                if (element->GetType() == 1) {
                    CBar* bar = static_cast<CBar*>(element);
                    bar->Value = value;
                    if (maxValue >= 0.0f) bar->MaxValue = maxValue;
                    if (minValue >= 0.0f) bar->MinValue = minValue;
                }
                return;
            }
        }
    }
};
int main()
{
    CBar bar; bar.ID = 4; COther other; other.ID = 5;
    CDrawQuestInfo q; q.Elements.push_back(0); q.Elements.push_back(&other); q.Elements.push_back(&bar);
    int ok = 1;
    q.UpdateBar(4, 2.5f, -1.0f, 0.0f);
    if (bar.Value != 2.5f || bar.MaxValue != 5.0f || bar.MinValue != 0.0f) ok = 0;
    q.UpdateBar(4, 3.0f, 9.0f, -2.0f);
    if (bar.Value != 3.0f || bar.MaxValue != 9.0f || bar.MinValue != 0.0f) ok = 0;
    q.UpdateBar(5, 7.0f, 7.0f, 7.0f);
    if (bar.Value != 3.0f) ok = 0;
    if (!ok) { std::printf("00644d91_TEST FAIL\n"); return 1; }
    std::printf("00644d91_TEST PASS\n");
    return 0;
}

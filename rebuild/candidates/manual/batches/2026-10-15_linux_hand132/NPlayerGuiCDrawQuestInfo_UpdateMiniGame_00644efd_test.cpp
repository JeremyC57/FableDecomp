#include <cstdio>
// Behaviour test for 00644efd.
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

// NPlayerGui::CDrawQuestInfo::UpdateMiniGame(float, long, long): percentage clamped to 1, stored on an updatable mini-game bar;
// a type-6 bar with exactly one extra element forwards the counter values to UpdateCounter.
struct CMiniGameBar : CQuestInfoElement {
    long Type; float Percentage; bool Updatable; long extra[4]; unsigned int extraCount;
    CMiniGameBar() : Type(6), Percentage(0), Updatable(true), extraCount(0) { }
    long GetType() { return 6; }
};
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
struct CDrawQuestInfo {
    CRetailElementList Elements; long MiniGameElementIndex;
    long lastId, lastValue, lastMax; int counterCalls;
    CDrawQuestInfo() : MiniGameElementIndex(0), counterCalls(0) { }
    void UpdateCounter(long id, long value, long maxValue) { ++counterCalls; lastId = id; lastValue = value; lastMax = maxValue; }
    void UpdateMiniGame(float percentage, long value, long maxValue)
    {
        long index = MiniGameElementIndex;
        if (index > 0) {
            percentage = Min(percentage, 1.0f);
            for (CRetailElementList::node* n = Elements.head->next; n != Elements.head; n = n->next) {
                CQuestInfoElement* element = n->value.m_pObject;
                if (element->ID == index) {
                    if (element->GetType() == 6) {
                        CMiniGameBar* bar = static_cast<CMiniGameBar*>(element);
                        if (bar->Updatable) {
                            bar->Percentage = percentage;
                            if (bar->Type == 6 && bar->extraCount == 1) UpdateCounter(bar->extra[0], value, maxValue);
                        }
                    }
                    return;
                }
            }
        }
    }
};
int main()
{
    CMiniGameBar bar; bar.ID = 3; bar.extra[0] = 11; bar.extraCount = 1;
    CDrawQuestInfo q; q.Elements.push_back(&bar);
    int ok = 1;
    q.UpdateMiniGame(0.5f, 1, 2);
    if (bar.Percentage != 0.0f || q.counterCalls != 0) ok = 0;
    q.MiniGameElementIndex = 3;
    q.UpdateMiniGame(1.5f, 4, 9);
    if (bar.Percentage != 1.0f || q.counterCalls != 1 || q.lastId != 11 || q.lastValue != 4 || q.lastMax != 9) ok = 0;
    bar.extraCount = 2; q.UpdateMiniGame(0.25f, 1, 1);
    if (bar.Percentage != 0.25f || q.counterCalls != 1) ok = 0;
    bar.Updatable = false; q.UpdateMiniGame(0.75f, 1, 1);
    if (bar.Percentage != 0.25f) ok = 0;
    if (!ok) { std::printf("00644efd_TEST FAIL\n"); return 1; }
    std::printf("00644efd_TEST PASS\n");
    return 0;
}

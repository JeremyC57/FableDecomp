#include <cstdio>
#include <cstring>
#include <cmath>

struct CRGBColour { unsigned char R, G, B, A; };
struct CRetailHandleVector { void* f; void* l; void* e; };
struct CQuestInfoElement { virtual void v00() {} virtual bool IsActive() { return active; } virtual void Clear() { cleared++; } virtual long GetType() { return type; }
    long ID; long ScriptID; float Scale; CRetailHandleVector PrimitiveHandles; long type; bool active; int cleared;
    CQuestInfoElement(long id, long t, long s = 0) : ID(id), ScriptID(s), type(t), active(true), cleared(0) {} };
struct CCountedElementPointer { CQuestInfoElement* m_pObject; void* m_pBlock; };
struct CRetailElementList {
    struct node { node* next; node* prev; CCountedElementPointer value; };
    struct iterator { node* m_pNode; explicit iterator(node* n) : m_pNode(n) {} CCountedElementPointer& operator*() const { return m_pNode->value; } iterator& operator++() { m_pNode = m_pNode->next; return *this; } bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; } bool operator==(const iterator& o) const { return m_pNode == o.m_pNode; } };
    node* head;
    CRetailElementList() { head = new node; head->next = head->prev = head; head->value.m_pObject = 0; }
    iterator begin() { return iterator(head->next); } iterator end() { return iterator(head); } bool empty() const { return head->next == head; }
    void push_back(CQuestInfoElement* e) { node* n = new node; n->value.m_pObject = e; n->value.m_pBlock = 0; n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
    iterator erase(iterator w) { node* n = w.m_pNode; node* nx = n->next; n->prev->next = nx; nx->prev = n->prev; delete n; return iterator(nx); }
    long count() { long c = 0; for (iterator i = begin(); i != end(); ++i) ++c; return c; }
};
static int g_display = -1; static long g_removed = -1;
struct CDrawQuestInfo { unsigned char _pad[0x28]; long GuiMode; float FadeInTimer, MiniModeTimer; bool MiniMode; long FramesUntilMiniMode; long MiniGameElementIndex; void* OracleSymbols; CRetailHandleVector OraclePrimitiveHandles; void* Name; void* Text; CRetailElementList Elements;
    void DisplayQuestInfo(bool d) { g_display = d; } void RemoveElement(long id) { g_removed = id; }
    CDrawQuestInfo() : GuiMode(0), MiniMode(false), FramesUntilMiniMode(0), MiniGameElementIndex(0) {}
    void UpdateCounter(long id, long v, long m);
};

struct CQuestInfoElementCounter : CQuestInfoElement { void* PCounter; long Graphic, Value, MaxValue; CQuestInfoElementCounter(long id, long t) : CQuestInfoElement(id, t), Value(0), MaxValue(0) {} };
void CDrawQuestInfo::UpdateCounter(long id, long value, long maxValue) {
    for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) { CQuestInfoElement* e = (*it).m_pObject;
        if (e != 0 && e->ID == id) { if (e->GetType() == 3) { CQuestInfoElementCounter* c = static_cast<CQuestInfoElementCounter*>(e); c->Value = value; if (maxValue >= 0) c->MaxValue = maxValue; } return; } } }
int main() {

    CDrawQuestInfo q; CQuestInfoElementCounter a(1, 4), b(2, 3); q.Elements.push_back(0); q.Elements.push_back(&a); q.Elements.push_back(&b);
    q.UpdateCounter(1, 5, 9); if (a.Value) { std::printf("00644e3c_TEST FAIL\n"); return 1; }
    q.UpdateCounter(2, 5, -1); if (b.Value != 5 || b.MaxValue != 0) { std::printf("00644e3c_TEST FAIL\n"); return 1; }
    q.UpdateCounter(2, 6, 9); if (b.Value != 6 || b.MaxValue != 9) { std::printf("00644e3c_TEST FAIL\n"); return 1; }
    std::printf("00644e3c_TEST PASS\n"); return 0;
}
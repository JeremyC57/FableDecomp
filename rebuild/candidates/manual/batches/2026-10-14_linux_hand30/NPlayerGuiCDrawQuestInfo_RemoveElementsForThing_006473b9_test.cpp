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
    void RemoveElementsForThing(const void* thing);
};

struct CBaseIntelligentPointer { void* item; void* info; void* GetPItem() { return item; } };
#pragma pack(push, 1)
struct CQuestInfoElementBar : CQuestInfoElement { float Value, MaxValue, MinValue; CRGBColour Colour, FadeColour; long Graphic1, Graphic2; bool ThingValid; CBaseIntelligentPointer PThing;
    CQuestInfoElementBar(long id, long t, bool v, void* th) : CQuestInfoElement(id, t), ThingValid(v) { PThing.item = th; } };
#pragma pack(pop)
void CDrawQuestInfo::RemoveElementsForThing(const void* thing) {
    for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) { CQuestInfoElement* e = (*it).m_pObject;
        if (e->GetType() == 1) { CQuestInfoElementBar* b = static_cast<CQuestInfoElementBar*>(e); if (b->ThingValid && b->PThing.GetPItem() == thing) { RemoveElement(e->ID); return; } } } }
int main() {

    int t1, t2; CDrawQuestInfo q; CQuestInfoElementBar a(1, 1, false, &t1), b(2, 1, true, &t1), c(3, 1, true, &t2); q.Elements.push_back(&a); q.Elements.push_back(&b); q.Elements.push_back(&c);
    q.RemoveElementsForThing(&t2); if (g_removed != 3) { std::printf("006473b9_TEST FAIL\n"); return 1; }
    q.RemoveElementsForThing(&t1); if (g_removed != 2) { std::printf("006473b9_TEST FAIL\n"); return 1; }
    std::printf("006473b9_TEST PASS\n"); return 0;
}

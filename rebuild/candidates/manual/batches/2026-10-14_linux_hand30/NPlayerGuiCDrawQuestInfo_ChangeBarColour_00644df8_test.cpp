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
    void ChangeBarColour(long id, const CRGBColour& c, const CRGBColour& f);
};

struct CQuestInfoElementBar : CQuestInfoElement { float Value, MaxValue, MinValue; CRGBColour Colour, FadeColour; CQuestInfoElementBar(long id, long t) : CQuestInfoElement(id, t) { Colour.A = 0; FadeColour.A = 0; } };
void CDrawQuestInfo::ChangeBarColour(long id, const CRGBColour& colour, const CRGBColour& fadeColour) {
    for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) { CQuestInfoElement* e = (*it).m_pObject;
        if (e != 0 && e->ID == id) { if (e->GetType() == 1) { CQuestInfoElementBar* b = static_cast<CQuestInfoElementBar*>(e); b->Colour = colour; b->FadeColour = fadeColour; } return; } } }
int main() {

    CDrawQuestInfo q; CQuestInfoElementBar a(1, 3), b(2, 1); q.Elements.push_back(0); q.Elements.push_back(&a); q.Elements.push_back(&b);
    CRGBColour c = {1,2,3,4}, f = {5,6,7,8}; q.ChangeBarColour(1, c, f); if (a.Colour.A != 0) { std::printf("00644df8_TEST FAIL\n"); return 1; }
    q.ChangeBarColour(2, c, f); if (b.Colour.A != 4 || b.FadeColour.A != 8) { std::printf("00644df8_TEST FAIL\n"); return 1; }
    std::printf("00644df8_TEST PASS\n"); return 0;
}

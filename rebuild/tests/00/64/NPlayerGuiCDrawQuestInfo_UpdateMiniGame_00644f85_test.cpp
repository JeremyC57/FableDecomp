#include <cstdio>
#include <cstring>
#include <cmath>

class CRGBColour { public: unsigned char R, G, B, A; };
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
    void UpdateMiniGame(const CRGBColour& c);
};

struct CQuestInfoElementMiniGameBar : CQuestInfoElement { long Type; float Percentage; CRGBColour Colour, FadeColour; bool Updatable; CQuestInfoElementMiniGameBar(long id, long t, bool u) : CQuestInfoElement(id, t), Updatable(u) { Colour.A = 0; } };
void CDrawQuestInfo::UpdateMiniGame(const CRGBColour& colour) {
    long index = MiniGameElementIndex;
    if (index > 0) { for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) { CQuestInfoElement* e = (*it).m_pObject;
        if (e->ID == index) { if (e->GetType() == 6) { CQuestInfoElementMiniGameBar* b = static_cast<CQuestInfoElementMiniGameBar*>(e); if (b->Updatable) b->Colour = colour; } return; } } } }
int main() {

    CDrawQuestInfo q; CQuestInfoElementMiniGameBar a(1, 6, false), b(2, 6, true); q.Elements.push_back(&a); q.Elements.push_back(&b);
    CRGBColour c = {1,2,3,4}; q.UpdateMiniGame(c); if (a.Colour.A || b.Colour.A) { std::printf("00644f85_TEST FAIL\n"); return 1; }
    q.MiniGameElementIndex = 1; q.UpdateMiniGame(c); if (a.Colour.A) { std::printf("00644f85_TEST FAIL\n"); return 1; }
    q.MiniGameElementIndex = 2; q.UpdateMiniGame(c); if (b.Colour.A != 4) { std::printf("00644f85_TEST FAIL\n"); return 1; }
    std::printf("00644f85_TEST PASS\n"); return 0;
}
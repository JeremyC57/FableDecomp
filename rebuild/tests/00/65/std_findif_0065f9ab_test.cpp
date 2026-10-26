#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing { public: bool m_isGuard; };
class CThingAICreature;
class CBaseIntelligentPointer {
public:
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* thing) { m_pItem = thing; }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { m_pItem = o.m_pItem; }
};
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;
    SListNode* prev;
    T value;
};
template <class T> class CListIterator {
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
// test helper: a circular list with a sentinel head
template <class T> struct STestList {
    SListNode<T> head;
    SListNode<T> nodes[8];
    int n;
    STestList() : n(0) { head.next = head.prev = &head; }
    void Add(const T& v)
    {
        SListNode<T>* node = &nodes[n++];
        node->value = v;
        node->prev = head.prev; node->next = &head;
        head.prev->next = node; head.prev = node;
    }
    CListIterator<T> begin() { return CListIterator<T>(head.next); }
    CListIterator<T> end() { return CListIterator<T>(&head); }
};
namespace std {
struct input_iterator_tag {};
template <class InputIter, class Predicate>
InputIter __fastcall __find_if(InputIter first, InputIter last, Predicate pred, const input_iterator_tag&)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
struct HasGuardProperty {
public:
    HasGuardProperty() {}
    bool operator()(CIntelligentPointer<CThingAICreature> creature) const { return creature.GetPItem() && creature.GetPItem()->m_isGuard; }
};
int main()
{
    CThing c[3]; c[0].m_isGuard = false; c[1].m_isGuard = true; c[2].m_isGuard = true;
    STestList<CIntelligentPointer<CThingAICreature> > l;
    l.Add(CIntelligentPointer<CThingAICreature>(0));
    for (int i = 0; i < 3; ++i) l.Add(CIntelligentPointer<CThingAICreature>(&c[i]));
    std::input_iterator_tag tag;
    CListIterator<CIntelligentPointer<CThingAICreature> > it = std::__find_if(l.begin(), l.end(), HasGuardProperty(), tag);
    if (it.m_node != &l.nodes[2]) { std::printf("0065f9ab_TEST FAIL\n"); return 1; }
    c[1].m_isGuard = c[2].m_isGuard = false;
    it = std::__find_if(l.begin(), l.end(), HasGuardProperty(), tag);
    if (it.m_node != &l.head) { std::printf("0065f9ab_TEST FAIL\n"); return 1; }
    std::printf("0065f9ab_TEST PASS\n");
    return 0;
}
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing { public: long m_x, m_y; };
class CThingBuilding;
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
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CNearestBuildingWithProperty {
public:
    long m_words[6];                              // [0] x, [1] y, [2] best distance, [3] best building, [4] visited, [5] copies
    CNearestBuildingWithProperty(long x, long y) { m_words[0] = x; m_words[1] = y; m_words[2] = 0x7fffffff; m_words[3] = -1; m_words[4] = 0; m_words[5] = 0; }
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& o)
    {
        for (int i = 0; i < 6; ++i) m_words[i] = o.m_words[i];
        ++m_words[5];
    }
    void operator()(const CIntelligentPointer<CThingBuilding>& building)
    {
        CThing* b = building.GetPItem();
        long dx = b->m_x - m_words[0], dy = b->m_y - m_words[1];
        long d = dx * dx + dy * dy;
        if (d < m_words[2]) { m_words[2] = d; m_words[3] = m_words[4]; }
        ++m_words[4];
    }
};
int main()
{
    CThing b[3]; b[0].m_x = 10; b[0].m_y = 0; b[1].m_x = 2; b[1].m_y = 1; b[2].m_x = -5; b[2].m_y = -5;
    STestList<CIntelligentPointer<CThingBuilding> > l;
    for (int i = 0; i < 3; ++i) l.Add(CIntelligentPointer<CThingBuilding>(&b[i]));
    CNearestBuildingWithProperty r = std::for_each(l.begin(), l.end(), CNearestBuildingWithProperty(0, 0));
    if (r.m_words[3] != 1 || r.m_words[2] != 5 || r.m_words[4] != 3 || r.m_words[5] < 1) { std::printf("0065f5d2_TEST FAIL\n"); return 1; }
    std::printf("0065f5d2_TEST PASS\n");
    return 0;
}
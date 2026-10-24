#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CThing { public: int m_id; };
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
struct CMarkerInfo {
    CIntelligentPointer<const CThing> PThing;
    long GraphicIndex;
    bool AutoMarker;
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
template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
class CIsMarkerBoundToThing {
public:
    const CThing* m_pThing;
    CIsMarkerBoundToThing(const CThing* thing) : m_pThing(thing) {}
    bool operator()(const CMarkerInfo& marker) const
    {
        return marker.PThing.GetPItem() == m_pThing && !marker.AutoMarker;
    }
};
int main()
{
    CThing t[2];
    STestList<CMarkerInfo> l;
    CMarkerInfo m;
    m.PThing.m_pItem = &t[0]; m.GraphicIndex = 0; m.AutoMarker = false; l.Add(m);
    m.PThing.m_pItem = &t[1]; m.GraphicIndex = 1; m.AutoMarker = true;  l.Add(m);
    m.PThing.m_pItem = &t[1]; m.GraphicIndex = 2; m.AutoMarker = false; l.Add(m);
    CListIterator<CMarkerInfo> it = std::find_if(l.begin(), l.end(), CIsMarkerBoundToThing(&t[1]));
    if (it.m_node != &l.nodes[2] || (*it).GraphicIndex != 2) { std::printf("0082c720_TEST FAIL\n"); return 1; }
    it = std::find_if(l.begin(), l.end(), CIsMarkerBoundToThing(0));
    if (it.m_node != &l.head) { std::printf("0082c720_TEST FAIL\n"); return 1; }
    std::printf("0082c720_TEST PASS\n");
    return 0;
}
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
class CVillageTaskHorn { public: CThing* m_pHorn; };
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    long* m_pInfo;
    CCountedPointer() : m_pObject(0), m_pInfo(0) {}
    CCountedPointer(T* object, long* info) : m_pObject(object), m_pInfo(info) {}
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo) { if (m_pInfo) ++*m_pInfo; }
    ~CCountedPointer() { if (m_pInfo) --*m_pInfo; }
    CCountedPointer& operator=(const CCountedPointer& o) { m_pObject = o.m_pObject; m_pInfo = o.m_pInfo; if (m_pInfo) ++*m_pInfo; return *this; }
    T* operator->() const { return m_pObject; }
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
class CIsHornTaskWithThisHorn {
public:
    CIntelligentPointer<CThing> m_horn;
    CIsHornTaskWithThisHorn(CThing* horn) : m_horn(horn) {}
    bool operator()(CCountedPointer<CVillageTaskHorn> task) const { return task->m_pHorn == m_horn.GetPItem(); }
};
int main()
{
    CThing h[2]; h[0].m_id = 0; h[1].m_id = 1;
    CVillageTaskHorn t[3]; t[0].m_pHorn = &h[0]; t[1].m_pHorn = &h[1]; t[2].m_pHorn = &h[1];
    long refs[3] = { 1, 1, 1 };
    STestList<CCountedPointer<CVillageTaskHorn> > l;
    for (int i = 0; i < 3; ++i) l.Add(CCountedPointer<CVillageTaskHorn>(&t[i], &refs[i]));
    for (int i = 0; i < 3; ++i) refs[i] = 1;
    std::input_iterator_tag tag;
    CListIterator<CCountedPointer<CVillageTaskHorn> > it = std::__find_if(l.begin(), l.end(), CIsHornTaskWithThisHorn(&h[1]), tag);
    if (it.m_node != &l.nodes[1] || refs[0] != 1 || refs[1] != 1 || refs[2] != 1) { std::printf("0065f96d_TEST FAIL\n"); return 1; }
    CThing other;
    it = std::__find_if(l.begin(), l.end(), CIsHornTaskWithThisHorn(&other), tag);
    if (it.m_node != &l.head) { std::printf("0065f96d_TEST FAIL\n"); return 1; }
    std::printf("0065f96d_TEST PASS\n");
    return 0;
}
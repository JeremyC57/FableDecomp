#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    void* m_pInfo;
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
template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
namespace NEntityEvents {
class CEventBase {
public:
    long m_type;
    long m_frame;
    CEventBase(long type, long frame) : m_type(type), m_frame(frame) {}
    virtual ~CEventBase() {}
    virtual long GetType() const { return m_type; }
};
class CIsEventOfTypeAfterFrame {
public:
    long m_type;
    long m_frame;
    CIsEventOfTypeAfterFrame(long type, long frame) : m_type(type), m_frame(frame) {}
    bool operator()(const CCountedPointer<CEventBase>& ev) const
    {
        return ev->GetType() == m_type && m_frame < ev->m_frame;
    }
};
}
int main()
{
    using namespace NEntityEvents;
    CEventBase e0(1, 50), e1(2, 5), e2(2, 20), e3(2, 30);
    CEventBase* ev[4] = { &e0, &e1, &e2, &e3 };
    STestList<CCountedPointer<CEventBase> > l;
    for (int i = 0; i < 4; ++i) { CCountedPointer<CEventBase> p; p.m_pObject = ev[i]; p.m_pInfo = 0; l.Add(p); }
    CListIterator<CCountedPointer<CEventBase> > it = std::find_if(l.begin(), l.end(), CIsEventOfTypeAfterFrame(2, 10));
    if (it.m_node != &l.nodes[2]) { std::printf("00811e20_TEST FAIL\n"); return 1; }
    it = std::find_if(l.begin(), l.end(), CIsEventOfTypeAfterFrame(2, 20));      // strictly after: 30
    if (it.m_node != &l.nodes[3]) { std::printf("00811e20_TEST FAIL\n"); return 1; }
    it = std::find_if(l.begin(), l.end(), CIsEventOfTypeAfterFrame(3, 0));
    if (it.m_node != &l.head) { std::printf("00811e20_TEST FAIL\n"); return 1; }
    std::printf("00811e20_TEST PASS\n");
    return 0;
}
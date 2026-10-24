#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
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
template <class Result, class T> class mem_fun_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T* p) const { return (p->*m_pf)(); }
};
}
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CTCPerceiveThing {
public:
    int m_indirect;
    CTCPerceiveThing() : m_indirect(0) {}
    void SetAsIndirectlyAwareOfPerceivedThing() { ++m_indirect; }
};
int main()
{
    CTCPerceiveThing p[3];
    STestList<CTCPerceiveThing*> l;
    l.Add(&p[0]); l.Add(&p[2]); l.Add(&p[0]);
    std::for_each(l.begin(), l.end(), std::mem_fun_t<void, CTCPerceiveThing>(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
    if (p[0].m_indirect != 2 || p[1].m_indirect != 0 || p[2].m_indirect != 1) { std::printf("0091b890_TEST FAIL\n"); return 1; }
    std::printf("0091b890_TEST PASS\n");
    return 0;
}
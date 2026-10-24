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
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CAnimSet {
public:
    float m_alpha;
    float m_previousAlpha;
    void SetAlpha(float alpha)
    {
        m_previousAlpha = m_alpha;
        m_alpha = alpha;
    }
};
class CMultiplyAnimSetAlphaBy {
public:
    float m_factor;
    CMultiplyAnimSetAlphaBy(float factor) : m_factor(factor) {}
    void operator()(CAnimSet& set) const
    {
        float alpha = set.m_alpha * m_factor;
        if (alpha < 0.0f)
            alpha = 0.0f;
        else if (alpha > 1.0f)
            alpha = 1.0f;
        set.SetAlpha(alpha);
    }
};
int main()
{
    STestList<CAnimSet> l;
    float a[3] = { 0.25f, 0.75f, -0.5f };
    for (int i = 0; i < 3; ++i) { CAnimSet s; s.m_alpha = a[i]; s.m_previousAlpha = 9.0f; l.Add(s); }
    CMultiplyAnimSetAlphaBy r = std::for_each(l.begin(), l.end(), CMultiplyAnimSetAlphaBy(2.0f));
    if (l.nodes[0].value.m_alpha != 0.5f || l.nodes[1].value.m_alpha != 1.0f || l.nodes[2].value.m_alpha != 0.0f) { std::printf("007100a0_TEST FAIL\n"); return 1; }
    if (l.nodes[0].value.m_previousAlpha != 0.25f || l.nodes[1].value.m_previousAlpha != 0.75f || l.nodes[2].value.m_previousAlpha != -0.5f) { std::printf("007100a0_TEST FAIL\n"); return 1; }
    if (r.m_factor != 2.0f) { std::printf("007100a0_TEST FAIL\n"); return 1; }
    std::printf("007100a0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CClass { public: int m_id; };
namespace std {
struct input_iterator_tag {};
}
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
template <class T> class CList {
public:
    typedef CListIterator<T> iterator;
    SListNode<T>* m_head;
    unsigned long m_size;
    SListNode<T> m_pool[8];
    int m_used;
    CList() : m_size(0), m_used(0) { m_head = &m_pool[m_used++]; m_head->next = m_head->prev = m_head; }
    iterator begin() { return iterator(m_head->next); }
    iterator end() { return iterator(m_head); }
    iterator insert(iterator where, const T& value)
    {
        SListNode<T>* node = &m_pool[m_used++];
        SListNode<T>* next = where.m_node;
        node->value = value;
        node->next = next; node->prev = next->prev;
        next->prev->next = node; next->prev = node;
        ++m_size;
        return iterator(node);
    }
    template <class It> void _Insert(iterator where, It first, It last, std::input_iterator_tag)
    {
        for ( ; first != last; ++first)
            insert(where, *first);
    }
};
int main()
{
    CClass c[4]; for (int i = 0; i < 4; ++i) c[i].m_id = i;
    CList<CClass*> src, dst;
    src.insert(src.end(), &c[0]); src.insert(src.end(), &c[1]); src.insert(src.end(), &c[2]);
    dst.insert(dst.end(), &c[3]);
    dst._Insert(dst.begin(), src.begin(), src.end(), std::input_iterator_tag());
    if (dst.m_size != 4) { std::printf("0053caa0_TEST FAIL\n"); return 1; }
    int want[4] = { 0, 1, 2, 3 }; int k = 0;
    for (CListIterator<CClass*> it = dst.begin(); it != dst.end(); ++it) if ((*it)->m_id != want[k++]) { std::printf("0053caa0_TEST FAIL\n"); return 1; }
    dst._Insert(dst.end(), src.end(), src.end(), std::input_iterator_tag());
    if (dst.m_size != 4 || src.m_size != 3) { std::printf("0053caa0_TEST FAIL\n"); return 1; }
    std::printf("0053caa0_TEST PASS\n");
    return 0;
}
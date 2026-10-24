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
    T* GetPtr() const { return m_pObject; }
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
class CCombatActionBase {
public:
    class CStrikeEvent {
    public:
        long m_startFrame;
        long m_endFrame;
        bool IsActiveOnFrame(long frame) const { return frame >= m_startFrame && frame <= m_endFrame; }
    };
};
class CGetActiveStrikeEventThatIsFirstToFinish {
public:
    long m_frame;
    const CCombatActionBase::CStrikeEvent* m_pBest;
    long m_bestEndFrame;
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : m_frame(frame), m_pBest(0), m_bestEndFrame(0x7fffffff) {}
    void operator()(const CCountedPointer<CCombatActionBase::CStrikeEvent>& ev)
    {
        if (ev->IsActiveOnFrame(m_frame) && ev->m_endFrame < m_bestEndFrame) {
            m_pBest = ev.GetPtr();
            m_bestEndFrame = ev->m_endFrame;
        }
    }
};
int main()
{
    typedef CCombatActionBase::CStrikeEvent E;
    E e[4]; long s[4] = { 0, 5, 8, 11 }; long en[4] = { 20, 15, 10, 30 };
    STestList<CCountedPointer<E> > l;
    for (int i = 0; i < 4; ++i) { e[i].m_startFrame = s[i]; e[i].m_endFrame = en[i]; CCountedPointer<E> p; p.m_pObject = &e[i]; p.m_pInfo = 0; l.Add(p); }
    CGetActiveStrikeEventThatIsFirstToFinish r = std::for_each(l.begin(), l.end(), CGetActiveStrikeEventThatIsFirstToFinish(9));
    if (r.m_pBest != &e[2] || r.m_bestEndFrame != 10 || r.m_frame != 9) { std::printf("0085a9c0_TEST FAIL\n"); return 1; }
    r = std::for_each(l.begin(), l.end(), CGetActiveStrikeEventThatIsFirstToFinish(10));   // end frame is inclusive
    if (r.m_pBest != &e[2]) { std::printf("0085a9c0_TEST FAIL\n"); return 1; }
    r = std::for_each(l.begin(), l.end(), CGetActiveStrikeEventThatIsFirstToFinish(40));
    if (r.m_pBest != 0 || r.m_bestEndFrame != 0x7fffffff) { std::printf("0085a9c0_TEST FAIL\n"); return 1; }
    std::printf("0085a9c0_TEST PASS\n");
    return 0;
}
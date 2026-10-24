// std::for_each(std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::const_iterator first, std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::const_iterator last, CGetActiveStrikeEventThatIsFirstToFinish f) -- among the strike events active on a frame, keeps the one that ends first
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085A960, 82 bytes (+ padding), `ret 14h`. 8 callers in CCombatActionBase (e.g. 0x00856E3B);
// 0x0085A9C0 is a byte-identical instantiation.
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: the active test is an inline CStrikeEvent::IsActiveOnFrame(frame) member -- written inline in the functor VC swaps
// edx/esi (event pointer in esi, end frame in edx); through the member the event stays in edx and the end frame in esi as retail.
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::const_iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
template <class T> class CCountedPointer {
public:
    T* m_pObject;                                 // +0x00
    void* m_pInfo;                                // +0x04
    T* operator->() const { return m_pObject; }
    T* GetPtr() const { return m_pObject; }
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
        unsigned char _pad[0xc];
        long m_startFrame;                        // +0x0C
        long m_endFrame;                          // +0x10
        bool IsActiveOnFrame(long frame) const { return frame >= m_startFrame && frame <= m_endFrame; }
    };
};
class CGetActiveStrikeEventThatIsFirstToFinish {
public:
    long m_frame;                                 // +0x00
    const CCombatActionBase::CStrikeEvent* m_pBest;   // +0x04
    long m_bestEndFrame;                          // +0x08
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : m_frame(frame), m_pBest(0), m_bestEndFrame(0x7fffffff) {}
    void operator()(const CCountedPointer<CCombatActionBase::CStrikeEvent>& ev)
    {
        if (ev->IsActiveOnFrame(m_frame) && ev->m_endFrame < m_bestEndFrame) {
            m_pBest = ev.GetPtr();
            m_bestEndFrame = ev->m_endFrame;
        }
    }
};
template CGetActiveStrikeEventThatIsFirstToFinish __fastcall std::for_each(CListIterator<CCountedPointer<CCombatActionBase::CStrikeEvent> >, CListIterator<CCountedPointer<CCombatActionBase::CStrikeEvent> >, CGetActiveStrikeEventThatIsFirstToFinish);
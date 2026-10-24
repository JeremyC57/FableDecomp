// std::find_if(std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator first, std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator last, NEntityEvents::CIsEventOfTypeAfterFrame pred) -- finds the first event of a type that happened after a frame
// Leaf: find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811E20, 73 bytes, `ret 10h`. Callers: CCombatActionBase::GetPActiveStrikeEvent 0x0081139D,
// 0x0081201D. Callee: the event's virtual slot 1 (GetType).
// SGI find_if (`while (first != last && !pred(*first)) ++first; return first;`), `__fastcall` with the hidden iterator result in ecx
// and the list iterators/predicate on the stack.
// Shape: `m_frame < ev->m_frame` (not `ev->m_frame > m_frame`) gives retail's `cmp edx,[ecx+4] ; jl` operand order.
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
    virtual ~CEventBase();
    virtual long GetType() const;                 // slot 1 (+0x04)
    long m_frame;                                 // +0x04
};
class CIsEventOfTypeAfterFrame {
public:
    long m_type;                                  // +0x00
    long m_frame;                                 // +0x04
    CIsEventOfTypeAfterFrame(long type, long frame) : m_type(type), m_frame(frame) {}
    bool operator()(const CCountedPointer<CEventBase>& ev) const
    {
        return ev->GetType() == m_type && m_frame < ev->m_frame;
    }
};
}
template CListIterator<CCountedPointer<NEntityEvents::CEventBase> > __fastcall std::find_if(CListIterator<CCountedPointer<NEntityEvents::CEventBase> >, CListIterator<CCountedPointer<NEntityEvents::CEventBase> >, NEntityEvents::CIsEventOfTypeAfterFrame);
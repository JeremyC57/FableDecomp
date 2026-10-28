// CTCEntityEvents::HasEventOfTypeCreatedAfterFrame(NEntityEvents::EEventType type, long frame) const -- whether the entity's event list holds an event of the type created after the frame
// Leaf: ?HasEventOfTypeCreatedAfterFrame@CTCEntityEvents@@QBE_NW4EEventType@NEntityEvents@@J@Z
// Retail: Fable.exe @ 0x00811380, 53 bytes, thiscall `ret 8`.
// Identity (proven): FableWin.pdb ?HasEventOfTypeCreatedAfterFrame@CTCEntityEvents@@QBE_NW4EEventType@NEntityEvents@@J@Z is
//   the only CTCEntityEvents bool(EEventType, long) const, and the PDB's only find_if over the CCountedPointer<CEventBase>
//   list with NEntityEvents::CIsEventOfTypeAfterFrame (Type, Frame; ctor (EEventType, long)) is the callee 0x00811E20.
//   Ego_r.pdb CTCEntityEvents::Events (the list) is +0x0C. The catalog label "CCombatActionBase::GetPActiveStrikeEvent"
//   is propagated (the callers 0x00659589 / 0x006595E2 / 0x006A8865 pass event type 0xe and a frame).
// Callees: STLport std::__find_if<list<CCountedPointer<CEventBase> >::const_iterator, CIsEventOfTypeAfterFrame>(first,
//   last, pred, const input_iterator_tag&) 0x00811E20 (__fastcall: the hidden iterator result in ecx, the tag reference
//   in edx, `ret 10h` for the two iterators and the 8-byte predicate). Landed as `find_if` (see NOTES "Review at landing").
// Shape: STLport `find_if` inlines to `__find_if(first, last, pred, iterator_category(first))`; the empty tag temporary
//   shares the dead `type` slot with the result (the old park's "dead lea edx" is the tag argument). Same-TU noinline
//   __find_if body (the landed 0x006BBFA0 model); the result in a named local so end() is re-read after the call.
template <class T> struct SListNode {
    SListNode* next;
    SListNode* prev;
    T value;
};
namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
}
template <class T> class CListIterator {
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
template <class T> inline std::bidirectional_iterator_tag iterator_category(const CListIterator<T>&) { return std::bidirectional_iterator_tag(); }
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    void* m_pInfo;
    T* operator->() const { return m_pObject; }
};
template <class T> class CEventList {
public:
    SListNode<T>* m_node;
    CListIterator<T> begin() const { return CListIterator<T>(m_node->next); }
    CListIterator<T> end() const { return CListIterator<T>(m_node); }
};
namespace std {
#pragma auto_inline(off)
template <class _InputIter, class _Predicate>
__declspec(noinline) _InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
#pragma auto_inline(on)
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, iterator_category(__first));
}
}
namespace NEntityEvents {
enum EEventType { EVENT_TYPE_PLACEHOLDER = 0 };
class CEventBase {
public:
    virtual ~CEventBase();
    virtual EEventType GetType() const;           // slot 1 (+0x04)
    long WorldFrame;                              // +0x04
};
class CIsEventOfTypeAfterFrame {
public:
    EEventType Type;                              // +0x00
    long Frame;                                   // +0x04
    CIsEventOfTypeAfterFrame(EEventType type, long frame) : Type(type), Frame(frame) {}
    bool operator()(const CCountedPointer<CEventBase>& ev) const
    {
        return ev->GetType() == Type && Frame < ev->WorldFrame;
    }
};
}
class CTCEntityEvents {
public:
    unsigned char _pad00[0x0c];
    CEventList<CCountedPointer<NEntityEvents::CEventBase> > Events;   // +0x0c
    bool HasEventOfTypeCreatedAfterFrame(NEntityEvents::EEventType type, long frame) const;
};
bool CTCEntityEvents::HasEventOfTypeCreatedAfterFrame(NEntityEvents::EEventType type, long frame) const
{
    CListIterator<CCountedPointer<NEntityEvents::CEventBase> > it =
        std::find_if(Events.begin(), Events.end(), NEntityEvents::CIsEventOfTypeAfterFrame(type, frame));
    return it != Events.end();
}
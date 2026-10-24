// CCombatActionBase::GetPActiveStrikeEvent() -- the action's active strike event that finishes first: runs the anonymous-namespace CGetActiveStrikeEventThatIsFirstToFinish functor over the strike-event list and returns the event it kept (null when none is active)
// Leaf: ?GetPActiveStrikeEvent@CCombatActionBase@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856E50, 70 bytes, __fastcall, plain `ret`.
// Identity: Ego_r.pdb ?GetPActiveStrikeEvent@CCombatActionBase@@IAEPAVCStrikeEvent@1@XZ (the non-const overload: the
//   callers 0x00858276 / 0x008584C1 push_back into the returned event's vector at +0x1c); Ego_r's for_each over the same
//   list names the functor CGetActiveStrikeEventThatIsFirstToFinish@?A0xad05d24c. The manifest label
//   "CCombatActionBase_GetFirstActiveStrikeEvent" is an autoname.
// Callee: std::for_each<list<CCountedPointer<CStrikeEvent> >::iterator, CGetActiveStrikeEventThatIsFirstToFinish>
//   0x0085A9C0 (__fastcall under /Gr: the hidden functor return in ecx, both iterators and the 12-byte functor by value).
// Layout: CCreatureActionBase FramesRunning +0x1c (rebuild/include/engine/CCreatureActionBase.h); the STLport strike-event
//   list (one sentinel pointer) at +0xbc. Functor {frame, kept event = 0, earliest finish = 1000000}.
class CCountedPointerInfo;
template <class T> class CCountedPointer {
public:
    T*                   Data;
    CCountedPointerInfo* Info;
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
};
template <class T> class allocator { };
template <class T, class A = allocator<T> > class list {     // STLport: one pointer to the sentinel node
public:
    typedef _List_iterator<T> iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _List_node_base* _M_node;
};
template <class I, class F> F __fastcall for_each(I first, I last, F f);   // 0x0085A9C0
}

class CCombatActionBase {
public:
    class CStrikeEvent;
protected:
    CStrikeEvent* GetPActiveStrikeEvent();
public:
    unsigned char _pad_0x00[0x1c];
    long          FramesRunning;                                 // +0x1c
    unsigned char _pad_0x20[0x9c];
    std::list<CCountedPointer<CStrikeEvent> > StrikeEvents;      // +0xbc
};

namespace {
class CGetActiveStrikeEventThatIsFirstToFinish {
public:
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : Frame(frame), PStrikeEvent(0), FirstFinishFrame(1000000) { }
    void operator()(const CCountedPointer<CCombatActionBase::CStrikeEvent>& strikeEvent);
    long                             Frame;
    CCombatActionBase::CStrikeEvent* PStrikeEvent;
    long                             FirstFinishFrame;
};
}

CCombatActionBase::CStrikeEvent* CCombatActionBase::GetPActiveStrikeEvent()
{
    return std::for_each(StrikeEvents.begin(), StrikeEvents.end(),
                         CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).PStrikeEvent;
}
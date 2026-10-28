// std::find_if(std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator first, std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator last, NEntityEvents::CIsEventOfTypeAfterFrame pred) -- finds the first event of a type that happened after a frame
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811E20, 73 bytes, `ret 10h`. Callers: CCombatActionBase::GetPActiveStrikeEvent 0x0081139D,
// 0x0081201D. Callee: the event's virtual slot 1 (GetType).
// SGI find_if (`while (first != last && !pred(*first)) ++first; return first;`), `__fastcall` with the hidden iterator result in ecx
// and the list iterators/predicate on the stack.
// Shape: `m_frame < ev->m_frame` (not `ev->m_frame > m_frame`) gives retail's `cmp edx,[ecx+4] ; jl` operand order.
namespace std {
struct input_iterator_tag { };
struct _List_node_base {                          // STLport 4.6 _list.h
    _List_node_base* _M_next;                     // +0x00
    _List_node_base* _M_prev;                     // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                  // +0x08
};
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    explicit _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
inline bool operator!=(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node != __y._M_node; }
}
template <class T> class CCountedPointer {
public:
    T* m_pObject;                                 // +0x00
    void* m_pInfo;                                // +0x04
    T* operator->() const { return m_pObject; }
};
namespace std {
template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)   // STLport 4.6 _algobase.c
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
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
typedef std::_List_iterator<CCountedPointer<NEntityEvents::CEventBase>, std::_Const_traits<CCountedPointer<NEntityEvents::CEventBase> > > CEventIter;
template CEventIter __fastcall std::__find_if(CEventIter, CEventIter, NEntityEvents::CIsEventOfTypeAfterFrame, const std::input_iterator_tag&);

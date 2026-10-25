// std::_Rb_tree<EEvent, EEvent, _Identity, less<EEvent> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const EEvent& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of NUISystem::CObserver's observed-event set (EEvent = NUISystem::EEvent): allocate a node, copy the event in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E0E0, 179 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x14-byte node: 0x10 links + the 4-byte event), _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (static, __cdecl).
// Callers: the set's insert_unique 0x0052E230 (landed: `Insert(&j, x, y, value, 0)` at 0x0052E283 / 0x0052E2BB), used by
//   NUISystem::CObserver::ObserveEvent 0x0052DA20 (landed).
// Label correction: catalogued as "CRedBlackTree_Insert_Int"; it is STLport `_Rb_tree::_M_insert` of the
//   set<NUISystem::EEvent> CObserver::EventsToObserve (Ego_r.pdb CObserver +0x04).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes =
//   { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xc, value +0x10 }; header parent = root, left = leftmost,
//   right = rightmost. The enum compare is signed (`jl`).
// Shape: STLport 4.x `_M_insert` verbatim, `_M_create_node` (malloc + null-tested placement copy) inlined in both arms.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace NUISystem {
enum EEvent { EVENT_NONE };
}
using NUISystem::EEvent;

struct _Rb_tree_node_base {
    unsigned char       _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0c
};

template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                               // +0x10
};

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

typedef _Rb_tree_node<EEvent> _Node;

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
};

class CEventSetTree {                               // STLport _Rb_tree<EEvent, EEvent, _Identity<EEvent>, less<EEvent> >
public:
    _Node*        _M_header;                        // +0x0
    unsigned int  _M_node_count;                    // +0x4
    less<EEvent>  _M_key_compare;                   // +0x8

    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const EEvent& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field; }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }

    _Node* _M_create_node(const EEvent& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_value_field) EEvent(__x);
        return __tmp;
    }

    _Rb_tree_iterator _M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const EEvent& __v,
                                _Rb_tree_node_base* __w_);
};

_Rb_tree_iterator CEventSetTree::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const EEvent& __v,
                                           _Rb_tree_node_base* __w_)
{
    _Node* __w = (_Node*)__w_;
    _Node* __x = (_Node*)__x_;
    _Node* __y = (_Node*)__y_;
    _Node* __z;

    if (__y == _M_header ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_Identity<EEvent>()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == _M_header) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header->_M_parent);
    ++_M_node_count;
    return _Rb_tree_iterator(__z);
}
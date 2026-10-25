// std::__copy(_Rb_tree_iterator first, _Rb_tree_iterator last, back_insert_iterator result, const input_iterator_tag&, int*) -- STLport input-iterator copy of a map<CCharString, int> into a vector<pair<CCharString, int> > through a back inserter: each node's pair is converted to a temporary pair<CCharString, int> and pushed back; returns the inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55880, 220 bytes, `ret 10h`, `and esp,-8` frame: __fastcall
//   returning the 4-byte back_insert_iterator through the hidden pointer in ecx, the tag reference in edx (unused), and
//   first, last, the inserter by value and the distance pointer on the stack.
// Callers: CGraphicDataBank::ChangeScene @ 0x00A50BF0 (call 0x00A50C16: copy(map.begin(), map.end(), back_inserter(v))),
//   0x00A5597A, 0x00A559AA. Callees: CCharString(const CCharString&) @ 0x0099EC30 (the converting pair ctor, then the
//   placement-new pair copy in push_back), ~CCharString @ 0x0099EAE0, vector<pair<CCharString, int> >::_M_insert_overflow
//   @ 0x006FC130 (landed).
// Label: the manifest's `BinaryTreeTraversal_InsertPair` is descriptive; STLport 4.6 `__copy(_InputIter, _InputIter,
//   _OutputIter, const input_iterator_tag&, _Distance*)`.
// Shape: `for (; first != last; ++result, ++first) *result = *first;` -- the back_insert_iterator::operator= takes
//   pair<CCharString, int>, so *first (pair<const CCharString, int>) goes through the converting template ctor into a
//   temporary; push_back inline (placement-new null test, `add [ebx+4],8`, else _M_insert_overflow with a __false_type
//   byte); _Rb_global::_M_increment inline for ++first.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

struct input_iterator_tag { input_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _T1, class _T2> class pair {
public:
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                      // +0x10
};

struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};

template <class _Value, class _Traits> struct _Rb_tree_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Self& __it) : _M_node(__it._M_node) { }
    typename _Traits::reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> class vector {
public:
    typedef _Tp value_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                  // 0x006FC130
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&,
                              _Distance*)
{
    for (; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}

typedef pair<const CCharString, int> _MapValue;
typedef _Rb_tree_iterator<_MapValue, _Nonconst_traits<_MapValue> > _MapIter;
typedef back_insert_iterator<vector<pair<CCharString, int> > > _Inserter;

template _Inserter __fastcall __copy<_MapIter, _Inserter, int>(_MapIter, _MapIter, _Inserter, const input_iterator_tag&,
                                                              int*);
}   // namespace std
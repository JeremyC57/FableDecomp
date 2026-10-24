// std::__copy<_Rb_tree_iterator<pair<const long, COpinionDeed*> >, back_insert_iterator<vector<pair<long, COpinionDeed*> > >, int>(first, last, back_insert_iterator result, const input_iterator_tag&, int*) -- STLport input-iterator copy: append every (handle, deed) entry of the deed map to the vector through push_back
// Leaf: ??$__copy@
// Grade: DIFFER(190v187) -- push_back scheduling only: retail compares the finish pointer straight against
//        `[edi+8]` and keeps both converted pair halves in ecx/edx for the placement copy; ours loads end_of_storage into
//        edx first, so the pair's second half is spilled to the temporary and re-read (`mov ecx,[esp+14h]`), 3 bytes
//        longer. Tried: plain pair (184, also with the in-place iterator walk), STLport's explicit pair copy
//        constructor (identical), the `_M_node = _Rb_global<bool>::_M_increment(_M_node)` step (fixes the walk and
//        the ebx/ebp choice, 190), `_STLP_alloc_proxy` end_of_storage (identical). next: the back_insert_iterator
//        operator= / pair conversion spelling that builds the temporary before the capacity test.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9B70, length 187 bytes (+ int3 padding), `__fastcall` (hidden
// back_insert_iterator result in ecx, the tag reference in edx; first, last, result and the null Distance* on the stack =
// `ret 10h`, a /Gr algorithm TU). Callers: 0x006C7423 inside CTCHeroOpinionDeedLog::OnSerialise 0x006C7370 (manifest
// "CDeedManager::TransferState"; Ego_r.pdb OpinionDeedMap map<long, COpinionDeed*> is the retail +0x0C map it copies into a
// vector before writing it) and the two out-of-line `std::copy` instances 0x006C9D20 / 0x006C9DB0. The manifest label
// "BinaryTree_InsertPairs" is descriptive.
// Callees: vector<pair<long, COpinionDeed*> >::_M_insert_overflow(pointer, const value_type&, const __false_type&,
// size_type, bool) @ 0x006C80E0 (landed; pushed as `(finish, value, tag, 1, true)`).
// Layout: STLport tree node {colour, parent +4, left +8, right +0xC, value pair +0x10/+0x14}; vector {start, finish,
// end_of_storage}; back_insert_iterator {container pointer}.
// Shape: STLport 4.x `__copy` for input iterators verbatim -- `for ( ; first != last; ++result, ++first) *result =
// *first;` -- with back_insert_iterator::operator= -> push_back inline (the map's pair<const long, X> converted to the
// vector's pair<long, X> in a frame temporary, `_Construct` behind its null test, else _M_insert_overflow with the
// __false_type tag homed in the dead `result` slot) and the step `_M_node = _Rb_global<bool>::_M_increment(_M_node)`
// inline (the in-place member walk keeps the node in esi and loses retail's eax copy).
inline void* operator new(unsigned int, void* p) { return p; }

class COpinionDeed;

namespace std {

struct input_iterator_tag { };
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }
};

template <class T> struct __type_traits { typedef __false_type is_POD_type; };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator { public: allocator() { } };

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T> class vector {
public:
    typedef T value_type;
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage; // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x006C80E0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);
    }
};

template <class Container> class back_insert_iterator {
protected:
    Container* container;
public:
    explicit back_insert_iterator(Container& x) : container(&x) { }
    back_insert_iterator& operator=(const typename Container::value_type& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};

inline bool operator!=(const _Rb_tree_base_iterator& x, const _Rb_tree_base_iterator& y) { return x._M_node != y._M_node; }

template <class V> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};

template <class InputIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(InputIter first, InputIter last, OutputIter result, const input_iterator_tag&, Distance*)
{
    for ( ; first != last; ++result, ++first)
        *result = *first;
    return result;
}

}

typedef std::pair<const long, COpinionDeed*> DeedMapValue;
typedef std::vector<std::pair<long, COpinionDeed*> > DeedVector;

template std::back_insert_iterator<DeedVector> __fastcall std::__copy<std::_Rb_tree_iterator<DeedMapValue>, std::back_insert_iterator<DeedVector>, int>(
    std::_Rb_tree_iterator<DeedMapValue> first, std::_Rb_tree_iterator<DeedMapValue> last,
    std::back_insert_iterator<DeedVector> result, const std::input_iterator_tag&, int*);

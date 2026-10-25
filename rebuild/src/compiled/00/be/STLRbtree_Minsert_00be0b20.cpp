// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- (the static map tree of pair<const long, CEngineStaticMapFileBlockManager::CFileBlock>) link a new file-block node under parent y (left when y is the header, or w is null and x is set or the key is smaller; else right), fix the header's root/leftmost/rightmost, rebalance and count it; returns the iterator through the hidden pointer
// Leaf: ?_M_insert@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0B20, 194 bytes (+ int3 padding), `__fastcall`, hidden iterator result + 4
// arguments (`ret 14h`). The manifest label "predicate_tree_insert_node" is descriptive. Callers: the landed insert_unique
// 0x00BE0BF0 (two sites).
// Callees: the landed _M_create_node @ 0x00BE0AC0 (out of line on the right-child path; the left-child path inlines it: malloc
// @ 0x00BFEA0E of 0x24 bytes + the placement copy of the pair), STLport _Rb_global<bool>::_Rebalance @ 0x0042971D (__cdecl:
// node, header root reference).
// Shape: STLport 4.x `_M_insert`: `if (y == header || (w == 0 && (x != 0 || key(v) < key(y))))` -> left (root/rightmost when
// y is the header, else leftmost), else right (rightmost); then parent = y, left = right = 0 (one zero register), rebalance,
// `++_M_node_count` (load/inc/store), `return iterator(z)` through the hidden pointer (returned in eax).
// Layout: as the landed _M_create_node 0x00BE0AC0 / erase 0x00BE0A50: node {color, parent, left, right} + pair at +0x10
// (key, CFileBlock RefCount / LowPriority byte / LoadingData Data, Info -- the copy bumps Info->RefCount).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

struct CCPPointerInfo {                                    // the counted pointer's shared block
    long  RefCount;                                        // +0
    void (__fastcall* DeleteFunction)(void* data);         // +4
    void* Data;                                            // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    void Reset() { DeleteData(); Info = 0; Data = 0; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
};

class CBankFileAsyncEntry;
class CBankFileAsyncData;

namespace std {
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };

struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                      // +0x10
};
template <class Dummy> class _Rb_global {
public:
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    _Rb_tree_iterator operator++(int) { _Rb_tree_iterator tmp = *this; _M_increment(); return tmp; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T1, class T2> inline void _Copy_Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>*     _Link_type;
    typedef _Rb_tree_iterator<V>  iterator;
    _Rb_tree_node_base* _M_header;                         // +0 (the alloc proxy's data)
    unsigned int        _M_node_count;                     // +4
    C                   _M_key_compare;                    // +8
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* x) { return x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* x) { return x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* x) { return x->_M_parent; }
    static const K& _S_key(_Rb_tree_node_base* x) { return ((_Link_type)x)->_M_value_field.first; }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Copy_Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const V& v, _Rb_tree_node_base* w_ = 0);
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void erase(iterator position)
    {
        _Link_type y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                         _M_header->_M_left, _M_header->_M_right);
        _Destroy(&y->_M_value_field);
        if (y != 0)
            free(y);
        --_M_node_count;
    }
};

template <class K, class T, class C, class A> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    void erase(iterator position) { _M_t.erase(position); }
};
}

class CEngineStaticMapFileBlockManager {
public:
    struct CFileBlock {
        long                                RefCount;      // +0x0
        bool                                LowPriority;   // +0x4
        CCountedPointer<CBankFileAsyncData> LoadingData;   // +0x8
    };
};

template <class K, class V, class KoV, class C, class A>
typename std::_Rb_tree<K, V, KoV, C, A>::iterator
std::_Rb_tree<K, V, KoV, C, A>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const V& v, _Rb_tree_node_base* w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;
    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(v.first, _S_key(y))))) {
        z = _M_create_node(v);
        _S_left(y) = z;
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        _S_right(y) = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    _S_parent(z) = y;
    _S_left(z) = 0;
    _S_right(z) = 0;
    _Rb_global_inst::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

typedef std::pair<const long, CEngineStaticMapFileBlockManager::CFileBlock> TBlockValue;
template class std::_Rb_tree<long, TBlockValue, std::_Select1st<TBlockValue>, std::less<long>, std::allocator<TBlockValue> >;
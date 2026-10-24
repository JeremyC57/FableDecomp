// std::_Rb_tree<void*, pair<void* const, CBulletTimeManager::CBulletTimeInfo>, ...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport: link a new bullet-time entry node left or right of y, fix the header links, rebalance, count it
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6960, length 208 bytes (203 + int3 padding), `ret 14h` (hidden iterator result, x, y, v, w).
// Identity: CBulletTimeManager::BulletTimeInfoMap (map<void*, CBulletTimeInfo>, 0x1c-byte nodes): called by insert_unique(v)
// 0x004C6A30 and insert_unique(hint, v) 0x004C6BC0, under the landed map operator[] 0x004C6E00 that
// CBulletTimeManager::SetBulletTimeSlowdownMultiplier 0x004C6170 calls. The catalog "ListNode_InsertSplineData" is a
// propagated label.
// Callees: malloc @ 0x00BFEA0E, _Rb_global<bool>::_Rebalance(node, root&) @ 0x0042971D (__cdecl).
// Shape: STLport 4.x _M_insert with `_M_create_node` inlined at both sites (the 12-byte value is a trivial three-dword
// copy, small enough for the inline budget; compare 0x004C4890 / 0x004C4F70, where the right-hand site stays out of line);
// keys compare unsigned (`jb`).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CBulletTimeManager {
public:
    class CBulletTimeInfo {                                  // Ego_r.pdb, 8 bytes
    public:
        float Multiplier;                                    // +0x0
        float SoundPitch;                                    // +0x4
    };
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0c
};

template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                        // +0x10
};

template <class B> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

template <class T> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
};

template <class T> struct allocator {};
template <class T> struct less {};
template <class P> struct _Select1st {};

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    typedef _Rb_tree_iterator<V> iterator;
    struct : allocator<V> { _Rb_tree_node_base* _M_data; } _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    C            _M_key_compare;                                        // +0x08

    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = static_cast<_Link_type>(malloc(sizeof(_Rb_tree_node<V>)));
        new (&tmp->_M_value_field) V(x);
        return tmp;
    }
    _Rb_tree_node_base*& _M_root() const { return _M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header._M_data->_M_right; }
    static const K& _S_key(_Rb_tree_node_base* x) { return static_cast<_Link_type>(x)->_M_value_field.first; }

    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const V& v, _Rb_tree_node_base* w);
};

template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::iterator
_Rb_tree<K, V, KoV, C, A>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const V& v, _Rb_tree_node_base* w_)
{
    _Link_type w = static_cast<_Link_type>(w_);
    _Link_type x = static_cast<_Link_type>(x_);
    _Link_type y = static_cast<_Link_type>(y_);
    _Link_type z;

    if (y == _M_header._M_data || (w == 0 && (x != 0 || v.first < _S_key(y)))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header._M_data) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, _M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

typedef pair<void* const, CBulletTimeManager::CBulletTimeInfo> _BulletTimeValue;
template class _Rb_tree<void*, _BulletTimeValue, _Select1st<_BulletTimeValue>, less<void*>, allocator<_BulletTimeValue> >;
}
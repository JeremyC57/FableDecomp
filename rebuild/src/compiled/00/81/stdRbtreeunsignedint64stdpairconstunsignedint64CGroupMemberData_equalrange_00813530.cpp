// std::_Rb_tree<unsigned __int64,std::pair<const unsigned __int64,CGroupMemberData>,...>::equal_range(const unsigned __int64& k) -- STLport equal_range on the creature group's member map: (lower_bound(k), upper_bound(k))
// Leaf: ?equal_range@?$_Rb_tree@
// Retail: Fable.exe @ 0x00813530, 70 bytes (+ int3 padding), `ret 8`: __fastcall (the tree in ecx, passed through to
//   lower_bound), the hidden pair result and the key reference on the stack.
// Caller: 0x00813653 (map::erase(const key&) 0x00813640). Callee: _M_lower_bound(const key&) 0x00813200 (catalogued
//   `BinaryTree_LowerBound`; returns the node pointer in eax).
// Identity: CTCCreatureGroup::Members = map<unsigned __int64, CGroupMemberData> (Ego_r.pdb `_Eqrange` of the same tree;
//   node key at +0x10/+0x14). Label: catalogued `BinaryTree_EqualRange` (descriptive).
// Shape: STLport `return pair<iterator,iterator>(lower_bound(k), upper_bound(k));` -- VC evaluates the arguments right to
//   left, so the inline upper_bound / _M_upper_bound walk (unsigned 64-bit `k < key(x)`: `cmp hi ; ja/jb ; cmp lo ; jae`) runs first and
//   the out-of-line lower_bound second.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThingAICreature;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                      // +0x04
    ~CBaseIntelligentPointer();                              // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CGroupMemberData {                                     // 0x14 bytes (Ego_r.pdb)
public:
    CIntelligentPointer<const CThingAICreature> PMember;     // +0x00
    bool IsActive;                                           // +0x08
    unsigned long JoinedFrame;                               // +0x0C
    unsigned long DeactivatedFrame;                          // +0x10
};

namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

struct _Rb_tree_node_base {
    bool _M_color;                                           // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                        // +0x10
};
template <class Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
        _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);                 // 0x0042A34E
};
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
template <class V> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator<V> operator++(int) { _Rb_tree_iterator<V> tmp = *this; _M_increment(); return tmp; }
    bool operator==(const _Rb_tree_iterator<V>& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_iterator<V>& y) const { return _M_node != y._M_node; }
};
template <class Key, class Value, class KeyOfValue, class Compare>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value>* _Link_type;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;                           // +0x00 (heap-allocated header node)
    unsigned int _M_node_count;                              // +0x04
    Compare _M_key_compare;
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Rb_tree_node_base* x);                    // 0x004DFB8F
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = _M_header;
            _M_root() = 0;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }
    void erase(iterator position)
    {
        _Link_type y = (_Link_type)_Rb_global<bool>::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                          _M_header->_M_left, _M_header->_M_right);
        y->_M_value_field.~Value();
        if (y != 0)
            free(y);
        --_M_node_count;
    }
    void erase(iterator first, iterator last);
    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Link_type)x)->_M_value_field); }
    _Rb_tree_node_base* _M_lower_bound(const Key& k) const;  // 0x00813200
    _Rb_tree_node_base* _M_upper_bound(const Key& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const Key& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const Key& k);
};
template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };
}
typedef std::pair<const unsigned __int64, CGroupMemberData> SMemberValue;
typedef std::_Rb_tree<unsigned __int64, SMemberValue, std::_Select1st<SMemberValue>, std::less<unsigned __int64> > CMemberTree;

template <class Key, class Value, class KeyOfValue, class Compare>
std::pair<typename std::_Rb_tree<Key, Value, KeyOfValue, Compare>::iterator, typename std::_Rb_tree<Key, Value, KeyOfValue, Compare>::iterator>
std::_Rb_tree<Key, Value, KeyOfValue, Compare>::equal_range(const Key& k)
{
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}
template std::pair<CMemberTree::iterator, CMemberTree::iterator> CMemberTree::equal_range(const unsigned __int64&);
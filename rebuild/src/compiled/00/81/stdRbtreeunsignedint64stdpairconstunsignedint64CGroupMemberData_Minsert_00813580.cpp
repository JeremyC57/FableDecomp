// std::_Rb_tree<unsigned __int64,std::pair<const unsigned __int64,CGroupMemberData>,...>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insertion under parent y for the creature group's member map
// Leaf: ?_M_insert@?$_Rb_tree@
// Retail: Fable.exe @ 0x00813580, 192 bytes, `ret 14h`: __fastcall (the tree -> ebx), the hidden iterator result, x, y,
//   the value reference and the hint w on the stack.
// Callers: 0x008136FB and 0x0081373D (insert_unique 0x008136A0). Callees: malloc 0x00BFEA0E (0x30-byte node), the
//   member pair's out-of-line copy constructor 0x00812840 (landed as SGroupMemberPair00812840), _Rb_global<bool>::
//   _Rebalance 0x0042971D (__cdecl).
// Identity: CTCCreatureGroup::Members = map<unsigned __int64, CGroupMemberData> (Ego_r.pdb `_Insert` of the same tree;
//   node key at +0x10/+0x14, value at +0x10). Label: catalogued `CMemoryAllocatorVariableSize_InsertNode` (wrong class).
// Shape: STLport 4.x `_M_insert` -- `if (y == header || (w == 0 && (x != 0 || comp(KeyOfValue()(v), key(y)))))` left
//   insert (root/rightmost or leftmost upkeep) else right insert (rightmost upkeep), `_M_create_node` (malloc, null-tested
//   placement copy) duplicated in both arms, parent/left/right links, rebalance, ++count, iterator(z).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThingAICreature;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                      // +0x04
    virtual ~CBaseIntelligentPointer();                              // 0x00A01C10
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
    pair(const pair<T1, T2>& o);                             // out of line: 0x00812840 for the member pair
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& val) { new (p) T1(val); }
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
private:
    void _M_erase(_Rb_tree_node_base* x);                    // 0x004DFB8F
public:
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
protected:
    _Link_type _M_create_node(const Value& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<Value>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const Value& v, _Rb_tree_node_base* w);
};
template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };
}
typedef std::pair<const unsigned __int64, CGroupMemberData> SMemberValue;
typedef std::_Rb_tree<unsigned __int64, SMemberValue, std::_Select1st<SMemberValue>, std::less<unsigned __int64> > CMemberTree;

template <class Key, class Value, class KeyOfValue, class Compare>
typename std::_Rb_tree<Key, Value, KeyOfValue, Compare>::iterator
std::_Rb_tree<Key, Value, KeyOfValue, Compare>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v,
                                                          _Rb_tree_node_base* w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;
    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(KeyOfValue()(v), _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
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
    _Rb_global<bool>::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}
template CMemberTree::iterator CMemberTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const SMemberValue&,
                                                      std::_Rb_tree_node_base*);
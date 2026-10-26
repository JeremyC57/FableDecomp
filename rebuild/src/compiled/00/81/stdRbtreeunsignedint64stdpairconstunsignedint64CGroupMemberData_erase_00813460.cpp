// std::_Rb_tree<unsigned __int64,std::pair<const unsigned __int64,CGroupMemberData>,...>::erase(iterator first, iterator last) -- STLport range erase on the creature group's member map: clear() for the whole range, else erase node by node
// Leaf: ?erase@?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@0@Z
// Retail: Fable.exe @ 0x00813460, 196 bytes (+ int3 padding), `ret 8`: __fastcall (the tree -> ebx), the two iterators by
//   value on the stack.
// Caller: 0x00813684 (map::erase(const key&) 0x00813640, an equal_range + range erase). Callees: _M_erase(root)
//   0x004DFB8F, _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (__cdecl), ~CBaseIntelligentPointer 0x00A01C10 (the
//   value's CGroupMemberData::PMember at node +0x18), free 0x00BFEA14.
// Identity: CTCCreatureGroup::Members = map<unsigned __int64, CGroupMemberData> (Ego_r.pdb; node 0x30: header +0, key
//   +0x10, CGroupMemberData +0x18 = PMember, IsActive +0x20, JoinedFrame +0x24, DeactivatedFrame +0x28). Label:
//   catalogued `LTextTreeWalkThrough_ClearRange` (descriptive).
// Shape: STLport `if (first == begin() && last == end()) clear(); else while (first != last) erase(first++);` with
//   clear(), erase(position) (rebalance, destroy the value, null-tested deallocate, --count) and
//   _Rb_tree_base_iterator::_M_increment all inlined.
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
    T1 first;
    T2 second;
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
};
template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };
}
typedef std::pair<const unsigned __int64, CGroupMemberData> SMemberValue;
typedef std::_Rb_tree<unsigned __int64, SMemberValue, std::_Select1st<SMemberValue>, std::less<unsigned __int64> > CMemberTree;

template <class Key, class Value, class KeyOfValue, class Compare>
void std::_Rb_tree<Key, Value, KeyOfValue, Compare>::erase(iterator first, iterator last)
{
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}
template void CMemberTree::erase(CMemberTree::iterator, CMemberTree::iterator);
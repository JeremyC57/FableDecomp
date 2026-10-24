// CTCCreatureGroupTag::FindNewTagger() -- pick the first active group member (in UID order) as the new tagger; no active member clears the tagger
// Leaf: ?FindNewTagger@CTCCreatureGroupTag@@AAE_NXZ
// Retail: Fable.exe @ 0x008137E0, 118 bytes (+ int3 padding), __fastcall, plain `ret`, bool in al.
// Name: FableWin.pdb `?FindNewTagger@CTCCreatureGroupTag@@AAE_NXZ` (private). Callers: CTCCreatureGroupTag::RemoveMember
//   0x00813884 and DeactivateMember 0x008138C4 (landed, which call it `ChooseNewTagger`).
// Layout: Ego_r.pdb CTCCreatureGroupTag TaggerUID +0x28 (u64), TaggedFrame +0x30; the CTCCreatureGroup base holds the
//   STLport map<unsigned __int64, CGroupMemberData> Members at +0x18 (node: key +0x10, CGroupMemberData +0x18 with
//   IsActive at node +0x20). Label: catalogued `CMemoryAllocatorVariableSize_FindNextActive` (wrong class).
// Shape: a map iterator loop with `_M_increment` inlined and end() re-read from `this` every pass (`cmp eax,[ecx+18h]`);
//   the hit copies the key into TaggerUID and returns true, the fall-out zeroes both dwords and returns false.
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
    _Rb_tree_iterator<V>& operator++() { _M_increment(); return *this; }
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
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
};
template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };
}
typedef std::pair<const unsigned __int64, CGroupMemberData> SMemberValue;
typedef std::_Rb_tree<unsigned __int64, SMemberValue, std::_Select1st<SMemberValue>, std::less<unsigned __int64> > CMemberTree;
namespace std {
template <class Key, class T, class Compare>
class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;                                          // header +0x00, node count +0x04, comparator +0x08
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}
class CThing;
class CTCCreatureGroup {
public:
    virtual void v00();
    CThing* ParentThing;                                     // +0x04 (CTCBase)
    bool ToBeRemoved;                                        // +0x08
    bool IsInGlobalUpdateTCs;                                // +0x09
    unsigned long MemberCount;                               // +0x0C
    unsigned long ActiveMemberCount;                         // +0x10
    bool ShutdownOnEmpty;                                    // +0x14
    std::map<unsigned __int64, CGroupMemberData, std::less<unsigned __int64> > Members;   // +0x18 (retail 12 bytes)
};
class CTCCreatureGroupTag : public CTCCreatureGroup {
public:
    unsigned __int64 TaggerUID;                              // +0x28
    unsigned long TaggedFrame;                               // +0x30
private:
    bool FindNewTagger();
};

bool CTCCreatureGroupTag::FindNewTagger()
{
    for (std::map<unsigned __int64, CGroupMemberData, std::less<unsigned __int64> >::iterator it = Members.begin();
         it != Members.end(); ++it) {
        if (it->second.IsActive) {
            TaggerUID = it->first;
            return true;
        }
    }
    TaggerUID = 0;
    return false;
}
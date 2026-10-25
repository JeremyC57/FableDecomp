// std::_Rb_tree<CCharString,pair<long,long>>::erase(const CCharString& key) -- erase every entry of the region -> (crime duration, fine) map with this key and return how many there were
// Grade: DIFFER(92v92) -- only the frame slots of the two by-value distance() iterator copies are swapped (retail: first at frame+0, last at +4; ours: last at +0); tried const-ref distance (reloads p after the call), const pair, named locals, template distance + iterator_category, base-iterator model, reversed-parameter helper
// Leaf: ?erase@_Rb_tree_CrimeMap@std@@QAEIABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756C60, 92 bytes (+4 int3), __thiscall `ret 4`.
// Callees: _Rb_tree::_M_upper_bound(const key&) @ 0x007564F0 and ::_M_lower_bound @ 0x00756100 (out of line, the node
//   pointer in eax; manifest `_Ubound`/`_Lbound` are Dinkumware-spelled propagations), __distance(const iterator&,
//   const iterator&, const input_iterator_tag&) @ 0x00755CB0 (__cdecl, landed as Distance_CRetailTreeIterator, which
//   ignores the tag), _Rb_tree::erase(iterator, iterator) @ 0x00756420.
// Label: the manifest's "StdMap_Erase" is descriptive; the tree is CTCHeroRememberCrimes' map<CCharString,pair<long,long>>
//   (its erase(first, last) 0x00756420 is the only callee that touches the 0x1C nodes).
// Shape: STLport `pair<iterator,iterator> p = equal_range(x); size_type n = distance(p.first, p.second);
//   erase(p.first, p.second); return n;` -- equal_range's pair(lower_bound(x), upper_bound(x)) evaluates its arguments
//   right to left (upper bound first, kept in edi); p lives in the 8-byte frame; distance's empty
//   bidirectional_iterator_tag temporary (user ctors, so no store) is homed in the dead `key` argument slot; VC7.1 pops
//   only 8 of the 12 argument bytes and reuses the last slot for erase's by-value iterators, which have a user copy
//   constructor so they are built in the argument area (`mov eax,esp ; push ecx ; mov [eax],edi`).
class CCharString {
public:
    void* PStringData;
};

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : forward_iterator_tag { bidirectional_iterator_tag() {} };

struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0C
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

struct _Rb_tree_iterator_CrimeMap {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator_CrimeMap(_Rb_tree_node_base* node) : _M_node(node) {}
    _Rb_tree_iterator_CrimeMap(const _Rb_tree_iterator_CrimeMap& it) : _M_node(it._M_node) {}
};

int __cdecl __distance(const _Rb_tree_iterator_CrimeMap& first, const _Rb_tree_iterator_CrimeMap& last,
                       const input_iterator_tag&);           // 0x00755CB0

inline int distance(_Rb_tree_iterator_CrimeMap first, _Rb_tree_iterator_CrimeMap last)
{
    return __distance(first, last, bidirectional_iterator_tag());
}

struct _Rb_tree_CrimeMap {
    typedef _Rb_tree_iterator_CrimeMap iterator;
    _Rb_tree_node_base* _M_header;                           // +0x00
    unsigned int        _M_node_count;                       // +0x04

    _Rb_tree_node_base* _M_lower_bound(const CCharString& key) const;   // 0x00756100
    _Rb_tree_node_base* _M_upper_bound(const CCharString& key) const;   // 0x007564F0
    iterator lower_bound(const CCharString& key) { return iterator(_M_lower_bound(key)); }
    iterator upper_bound(const CCharString& key) { return iterator(_M_upper_bound(key)); }
    pair<iterator, iterator> equal_range(const CCharString& key)
    {
        return pair<iterator, iterator>(lower_bound(key), upper_bound(key));
    }
    void erase(iterator first, iterator last);               // 0x00756420
    unsigned int erase(const CCharString& key);
};

unsigned int _Rb_tree_CrimeMap::erase(const CCharString& key)
{
    pair<iterator, iterator> p = equal_range(key);
    unsigned int n = distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
}
}

// _STL::map<unsigned __int64, pair<CCharString, CWideString> >::operator[](const unsigned __int64& k) -- STLport map subscript of CThingManager::NPCNameMap: find the first node not below k; unless its key equals k insert (k, empty name pair) there; return a reference to the mapped (name, display name) pair
// Leaf: ??ACNPCNameMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052B8B0, 178 bytes, `ret 4` (thiscall: &k).
// Callees: _M_lower_bound(const key&) const 0x00528610 (out of line; manifest "BinaryTree_LowerBound_Uint64"),
//   insert_unique(iterator hint, const value_type&) 0x0052A2D0 (the iterator result through the hidden pointer, homed
//   in the dead `k` argument slot), CCharString() 0x0099E4B0, CWideString() 0x0099AED0, CCharString copy ctor
//   0x0099EC30, CWideString copy ctor 0x0099B720, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0.
// Callers: CThingManager 0x00521AB3 / 0x00521ACB (SetNameForThing), 0x005256BA, 0x00525BF6.
// Label correction: catalogued as "CParticleMorphs_Load"; it is STLport `map::operator[]` of
//   map<unsigned __int64, pair<CCharString, CWideString> > (Ego_r.pdb CThingManager::NPCNameMap).
// Layout: STLport map = its `_Rb_tree` { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes
//   { colour, parent, left, right, value +0x10: the UID, CCharString +0x18, CWideString +0x1c } -- the mapped pair is
//   node +0x18.
// Shape: STLport 4.x `iterator __i = lower_bound(__k); if (__i == end() || key_comp()(__k, (*__i).first)) __i =
//   insert(__i, value_type(__k, _Tp())); return (*__i).second;` -- `_Tp()` is built first (two default constructors),
//   then the value_type temporary (key words, then the inner pair's memberwise copy); the hint iterator is passed
//   by value through its user copy constructor (built in the argument area); the temporaries die in reverse order.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PData;                                            // CBasicString<char>* (4 bytes)
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CWideString {
public:
    void* PData;                                            // CBasicString<wchar_t>* (4 bytes)
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<CCharString, CWideString> SNPCName;
typedef pair<const unsigned __int64, SNPCName> SNPCNamePair;

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

struct SNPCNameNode : _Rb_tree_node_base {
    SNPCNamePair _M_value_field;                            // +0x10
};

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    SNPCNamePair& operator*() const { return ((SNPCNameNode*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
};

class CNPCNameTree {                                        // _Rb_tree<unsigned __int64, pair<const unsigned __int64, SNPCName>, ...>
public:
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base*     _M_header;                      // +0x00
    unsigned long           _M_node_count;                  // +0x04
    less<unsigned __int64>  _M_key_compare;                 // +0x08

    _Rb_tree_node_base* _M_lower_bound(const unsigned __int64& k) const;   // 0x00528610
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    iterator lower_bound(const unsigned __int64& k) { return iterator(_M_lower_bound(k)); }
    iterator insert_unique(iterator position, const SNPCNamePair& v);      // 0x0052A2D0
    less<unsigned __int64> key_comp() const { return _M_key_compare; }
};

class CNPCNameMap {                                         // map<unsigned __int64, pair<CCharString, CWideString> >
public:
    typedef CNPCNameTree::iterator iterator;
    CNPCNameTree _M_t;                                      // +0x00
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const unsigned __int64& k) { return _M_t.lower_bound(k); }
    iterator insert(iterator position, const SNPCNamePair& x) { return _M_t.insert_unique(position, x); }
    less<unsigned __int64> key_comp() const { return _M_t.key_comp(); }
    SNPCName& operator[](const unsigned __int64& k);
};

SNPCName& CNPCNameMap::operator[](const unsigned __int64& k)
{
    iterator i = lower_bound(k);
    // i->first is greater than or equivalent to k.
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, SNPCNamePair(k, SNPCName()));
    return (*i).second;
}
// std::map<CCharString,NBankFileManager::CContainedBank>::operator[](const CCharString& k) -- STLport subscript on a retail bank's contained-bank map: lower_bound, insert (k, CContainedBank()) when absent, return the value
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC530, 156 bytes, `ret 4`.
// Leaf: ??A?$map
// Name: FableWin.pdb ??A?$map@VCCharString@@VCContainedBank@NBankFileManager@@...@std@@QAEAAVCContainedBank@NBankFileManager@@ABVCCharString@@@Z
//       (the catalog's "CContainedBankMap::operator[]" is the map typedef); the map is CRetailBank::ContainedBanks (+0x18, ctor
//       0x009AC4D0). Only caller 0x009A8D87.
// Callees: _M_lower_bound @ 0x009AAD60 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950 (landed), CCharString copy
//          ctor @ 0x0099EC30 and dtor @ 0x0099EAE0 (the value_type temporary), _Rb_tree::insert_unique(iterator, const value_type&) @ 0x009AB870
//          (iterator argument built in its slot, the result returned through the dead `k` slot).
// Shape: STLport 4.x verbatim, as the landed map<CCharString, unsigned long>::operator[] 0x0042D131 -- but in an /O2 TU, where
//        less<CCharString> and CCharString::operator< are inline (pointer-equality, null tests, then the CBasicString compare);
//        CContainedBank() (FableWin.pdb has a user default ctor) zeroes the five words of the pair's second; map::insert is the
//        STLport inline forwarder to the tree's insert_unique, whose by-value result is copied back from its slot (a direct
//        out-of-line insert reads the result through eax instead).
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};
class CCharString {
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};
namespace NBankFileManager {
class CContainedBank { public:
    unsigned long DataVersion;                           // +0x00
    unsigned long EntryCount;                            // +0x04
    unsigned long InfoOffset;                            // +0x08
    unsigned long InfoSize;                              // +0x0c
    unsigned long Alignment;                             // +0x10
    CContainedBank() : DataVersion(0), EntryCount(0), InfoOffset(0), InfoSize(0), Alignment(0) {}
};
}
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;        // 0x009AAD60
    iterator insert_unique(iterator position, const value_type& v);   // 0x009AB870 (_Rb_tree::insert_unique)
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
template NBankFileManager::CContainedBank& map<CCharString, NBankFileManager::CContainedBank>::operator[](const CCharString&);
}
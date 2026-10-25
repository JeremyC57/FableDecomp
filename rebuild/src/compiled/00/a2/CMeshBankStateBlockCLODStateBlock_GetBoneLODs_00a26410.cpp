// CMeshBankStateBlock::CLODStateBlock::GetBoneLODs(set<CCharString>& bones, long lod) -- collect the names of every bone assigned to the given LOD
// Leaf: ?GetBoneLODs@CLODStateBlock@CMeshBankStateBlock@@QAEXAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26410, 164 bytes, `ret 8`. No direct caller (reached through a pointer).
// Identity: Ego_r.pdb `?GetBoneLODs@CLODStateBlock@CMeshBankStateBlock@@QAEXAAV?$set@VCCharString@@...@std@@J@Z`:
//   `this` is the CLODStateBlock whose BoneLODs (map<CCharString, long>) sits at +0 (see 0x00A2CFA0); the catalog
//   label "LTextTree_Insert" is descriptive.
// Callees: _Rb_tree::_M_erase 0x004ABA70 (set<CCharString> clear; manifest "LTextTreeWalkThrough::BuildTreeArray" is
//   propagated) and _Rb_tree::insert_unique 0x004AC380 (returns pair<iterator,bool> through the 8-byte frame).
// Shape: STLport set::clear() inlined (count test, _M_erase(root), leftmost/root/rightmost/count reset), then the map
//   walk with `it->second == lod` -> bones.insert(it->first). STLport's pair has user constructors, so
//   pair<iterator,bool> comes back through a hidden pointer (an aggregate pair returns in eax:edx and loses the
//   frame); the increment is the STLport 4.6 static `_Rb_global::_M_increment(node)` (walk on a copy in eax).
class CCharString { public: void* PStringData; };

namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() {} };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global::_M_increment(_M_node); }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }
template <class _Key, class _Value> class _Rb_tree {                // header, count, empty compare: 12 bytes
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Link_type __x);                                   // 0x004ABA70
    pair<iterator, bool> insert_unique(const _Value& __v);          // 0x004AC380
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = _M_header;
            _M_root() = 0;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    typedef _Rb_tree<_Key, _Key> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    void clear() { _M_t.clear(); }
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}

class CMeshBankStateBlock {
public:
    class CLODStateBlock {
    public:
        std::map<CCharString, long> BoneLODs;           // +0x00
        void GetBoneLODs(std::set<CCharString>& bones, long lod);
    };
};

void CMeshBankStateBlock::CLODStateBlock::GetBoneLODs(std::set<CCharString>& bones, long lod)
{
    bones.clear();
    for (std::map<CCharString, long>::iterator it = BoneLODs.begin(); it != BoneLODs.end(); ++it)
        if (it->second == lod)
            bones.insert(it->first);
}
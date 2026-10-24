// Behaviour test for CMeshBankStateBlock::CLODStateBlock::GetBoneLODs (0x00A26410): the output set is cleared first,
// then receives exactly the bones whose LOD equals the requested one (no duplicates, others untouched).
#include <cstdio>
#include <cstdlib>
#include <new>

class CCharString {
public:
    int Id;
    bool operator<(const CCharString& o) const { return Id < o.Id; }
};

static int g_erased = 0;

namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() {} };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
inline const CCharString& _KeyOf(const CCharString& k) { return k; }
template <class _Tp> inline const CCharString& _KeyOf(const pair<const CCharString, _Tp>& p) { return p.first; }
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
    _Rb_tree_node(const _Value& v) : _M_value_field(v) {}
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
template <class _Key, class _Value> class _Rb_tree {
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
    void Init()
    {
        _M_header = (_Rb_tree_node_base*)std::malloc(sizeof(_Rb_tree_node_base));
        _M_header->_M_color = 0; _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header; _M_header->_M_right = _M_header; _M_node_count = 0;
    }
    void _M_erase(_Link_type __x)
    {
        while (__x != 0) {
            _M_erase((_Link_type)__x->_M_right);
            _Link_type __y = (_Link_type)__x->_M_left;
            __x->~_Rb_tree_node<_Value>();
            std::free(__x);
            ++g_erased;
            __x = __y;
        }
    }
    pair<iterator, bool> insert_unique(const _Value& __v)                // unbalanced stand-in for the out-of-line insert
    {
        _Rb_tree_node_base* __y = _M_header;
        _Rb_tree_node_base* __x = _M_root();
        while (__x) {
            __y = __x;
            const CCharString& k = _KeyOf(((_Link_type)__x)->_M_value_field);
            if (_KeyOf(__v) < k) __x = __x->_M_left;
            else if (k < _KeyOf(__v)) __x = __x->_M_right;
            else return pair<iterator, bool>(iterator(__x), false);
        }
        _Link_type __z = new (std::malloc(sizeof(_Rb_tree_node<_Value>))) _Rb_tree_node<_Value>(__v);
        __z->_M_left = 0; __z->_M_right = 0; __z->_M_parent = __y; __z->_M_color = 0;
        if (__y == _M_header) { _M_root() = __z; _M_leftmost() = __z; _M_rightmost() = __z; }
        else if (_KeyOf(__v) < _KeyOf(((_Link_type)__y)->_M_value_field)) { __y->_M_left = __z; if (__y == _M_leftmost()) _M_leftmost() = __z; }
        else { __y->_M_right = __z; if (__y == _M_rightmost()) _M_rightmost() = __z; }
        ++_M_node_count;
        return pair<iterator, bool>(iterator(__z), true);
    }
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
        _STL::map<CCharString, long> BoneLODs;
        void GetBoneLODs(_STL::set<CCharString>& bones, long lod);
    };
};

void CMeshBankStateBlock::CLODStateBlock::GetBoneLODs(_STL::set<CCharString>& bones, long lod)
{
    bones.clear();
    for (_STL::map<CCharString, long>::iterator it = BoneLODs.begin(); it != BoneLODs.end(); ++it)
        if (it->second == lod)
            bones.insert(it->first);
}

int main()
{
    int ok = 1;
    CMeshBankStateBlock::CLODStateBlock block;
    block.BoneLODs._M_t.Init();
    int ids[7] = { 40, 10, 70, 20, 60, 30, 50 };
    long lods[7] = { 1, 2, 1, 0, 2, 1, 0 };
    for (int i = 0; i < 7; ++i) {
        CCharString name; name.Id = ids[i];
        block.BoneLODs._M_t.insert_unique(_STL::pair<const CCharString, long>(name, lods[i]));
    }
    _STL::set<CCharString> bones;
    bones._M_t.Init();
    CCharString stale; stale.Id = 999;
    bones.insert(stale);

    block.GetBoneLODs(bones, 1);                                    // 30, 40, 70
    if (g_erased != 1 || bones._M_t._M_node_count != 3) ok = 0;
    int expect1[3] = { 30, 40, 70 }, k = 0;
    for (_STL::set<CCharString>::iterator it = bones._M_t.begin(); it != bones._M_t.end(); ++it, ++k)
        if (k >= 3 || it->Id != expect1[k]) ok = 0;
    if (k != 3) ok = 0;
    block.GetBoneLODs(bones, 0);                                    // 20, 50
    int expect0[2] = { 20, 50 }; k = 0;
    for (_STL::set<CCharString>::iterator it = bones._M_t.begin(); it != bones._M_t.end(); ++it, ++k)
        if (k >= 2 || it->Id != expect0[k]) ok = 0;
    if (k != 2 || bones._M_t._M_node_count != 2 || g_erased != 4) ok = 0;
    block.GetBoneLODs(bones, 5);                                    // none
    if (bones._M_t._M_node_count != 0 || bones._M_t._M_header->_M_left != bones._M_t._M_header || bones._M_t._M_header->_M_parent != 0) ok = 0;
    if (block.BoneLODs._M_t._M_node_count != 7) ok = 0;

    if (!ok) { std::printf("00a26410_TEST FAIL\n"); return 1; }
    std::printf("00a26410_TEST PASS\n");
    return 0;
}
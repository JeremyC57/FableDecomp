// Behaviour test for 0x00A38550 CCRCSymbolMap::Clear: clear the CRC-keyed vector-map and the set of used CRCs
// (the set's erase runs only when it holds nodes, then the header is relinked empty)
#include <stdio.h>
static int gMapClears = 0;
static int gErases = 0;
static void* gErasedRoot = 0;
struct CBase {
    CBase() {}
    virtual ~CBase() {}
};
template <class K, class V> struct CKeyPairCompareLess { };
namespace _STL {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp*, unsigned int) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x) { gErasedRoot = __x; ++gErases; }      // stand-in
    void clear() {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Tp> struct _Identity { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    void clear() { _M_t.clear(); }
};
}
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V, class C> struct CVectorMap {
    CVector<CKeyValuePair<K, V> > Data;
    C                             Compare;
    bool                          Dirty;
    void Clear() { ++gMapClears; Data._M_finish = Data._M_start; }   // stand-in
};
struct CCRCSymbolMap : CBase {
    CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > SLONGMap;
    _STL::set<unsigned long>                                                  NamesUsed;
    bool                                                                      CheckForDuplicateSymbols;
    virtual ~CCRCSymbolMap() {}
    void Clear();
};
void CCRCSymbolMap::Clear()
{
    SLONGMap.Clear();
    NamesUsed.clear();
}
typedef _STL::_Rb_tree_node<unsigned long> CNameNode;
static CNameNode gHeader;
static CNameNode gRoot;
static CKeyValuePair<unsigned long, long> gEntries[2];
int main()
{
    CCRCSymbolMap m;
    m.SLONGMap.Data._M_start = gEntries;
    m.SLONGMap.Data._M_finish = gEntries + 2;
    m.NamesUsed._M_t._M_header._M_data = &gHeader;
    gHeader._M_parent = &gRoot; gHeader._M_left = &gRoot; gHeader._M_right = &gRoot;
    m.NamesUsed._M_t._M_node_count = 1;
    m.CheckForDuplicateSymbols = true;
    m.Clear();
    bool ok = true;
    ok = ok && gMapClears == 1 && m.SLONGMap.Data._M_finish == gEntries;
    ok = ok && gErases == 1 && gErasedRoot == (void*)&gRoot;
    ok = ok && gHeader._M_parent == 0 && gHeader._M_left == &gHeader && gHeader._M_right == &gHeader;
    ok = ok && m.NamesUsed._M_t._M_node_count == 0 && m.CheckForDuplicateSymbols;
    m.Clear();                                         // empty set: the map is still cleared, no erase
    ok = ok && gMapClears == 2 && gErases == 1;
    if (ok) { printf("00a38550_TEST PASS\n"); return 0; }
    printf("00a38550_TEST FAIL\n");
    return 1;
}
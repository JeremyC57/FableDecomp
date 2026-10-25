// map::operator[](const CDateAndTime& k) -- STLport subscript on a date-keyed wide-string table: lower_bound, insert (k, CWideString()) when absent, return the string
// Grade: RELOCATION_MATCH (agent902) -- STLport's `_STLP_DEFAULT_CONSTRUCTED(_Tp)` as the inline `__default_constructed((_Tp*)0)` (the landed 0x009EFCF0 layer): the default CWideString is returned into its frame slot and re-addressed (`lea eax,[ebp-4] ; push eax`) instead of pushing the ctor's eax.
// Leaf: ??A?$map@VCDateAndTime
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594A08, 107 bytes, `ret 4`.
// Name: Ego_r.pdb / FableWin.pdb `??A?$map@VCDateAndTime@@VCWideString@@U?$less@VCDateAndTime@@@std@@...@std@@QAEAAVCWideString@@ABVCDateAndTime@@@Z`.
// Callees: _Rb_tree::_M_lower_bound(k) @ 0x0059339A (this chunk; node pointer in eax), CDateAndTime::operator<
// const @ 0x009E9B80 (the inline less<CDateAndTime>), CWideString() @ 0x0099AED0, pair<const CDateAndTime,
// CWideString>::pair(const CDateAndTime&, const CWideString&) @ 0x005921A4 (out of line: a 7-dword copy + the
// CWideString copy ctor), map::insert(iterator, const value_type&) @ 0x0059483E (forwards to the tree's hinted
// insert_unique 0x00594481), ~CWideString @ 0x0099B510 (the pair temporary's value, then the default value).
// Only caller 0x0058FB22 (CTCInventory TU).
// Label: the catalog "BinarySearchTree_FindAndConstruct_DateAndTime" is descriptive: the body is STLport 4.x
// `map::operator[]` verbatim (the node value is node + 0x2C: key CDateAndTime 0x1C bytes at node + 0x10).
// Shape: this in edi, the node in esi; the default value [ebp-4], the insert result [ebp-8] and the 0x20-byte
// value_type temporary [ebp-28h] in the frame; the iterator argument rebuilt in its slot by its copy ctor.
// Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

class CDateAndTime {
public:
    unsigned char _body[0x1c];                       // 0x1C bytes: the CWideString value follows at node + 0x2C
    bool operator<(const CDateAndTime& other) const; // 0x009E9B80
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                   // 0x0099AED0
    ~CWideString();                                  // 0x0099B510
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b);            // 0x005921A4
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                   // +0x00 (_M_t._M_header._M_data)
    size_t              _M_node_count;               // +0x04
    _Compare            _M_key_compare;              // +0x08
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x0059339A
    iterator insert(iterator __position, const value_type& __x);       // 0x0059483E
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};
template CWideString& map<CDateAndTime, CWideString>::operator[](const CDateAndTime&);
}
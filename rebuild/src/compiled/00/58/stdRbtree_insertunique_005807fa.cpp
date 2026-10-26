// std::_Rb_tree::insert_unique(const value_type& v) -- (map<CCharString, CTCHeroStats::MirroredCriminalStatus >) STLport unique insert: walk to the leaf, step back once when v went left of a non-leftmost node, insert unless the predecessor's key equals v's; returns (iterator, inserted)
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005807FA, 166 bytes, thiscall `ret 8` (hidden pair<iterator, bool> result + v), ebp frame (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::_Rb_tree<CCharString, pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>, _Select1st<...>, less<CCharString>, allocator<...> >::insert_unique(const value_type&); evidence: the tree is map<CCharString, CTCHeroStats::MirroredCriminalStatus> (CTCHeroStats::OutstandingCrimes): _M_insert 0x00580765 / node factory 0x00580407 (0x58-byte node, value copy 0x0057F30A; family J044); the same tree's _M_lower_bound 0x0057FE4A serves operator[] 0x005812DF (family J042); this body calls that tree's _M_insert 0x00580765; its call sites: 0x00580D0E
// Label: the manifest's "BinarySearchTree_FindInsertionPoint" is a descriptive placeholder (family J045: five byte-similar insert_unique bodies over CCharString keys, each its own
//   COMDAT; retail has no identical-code folding, so each address is one tree type).
// Callees: std::less<CCharString>::operator() 0x0042AAB3 (??R?$less@VCCharString@@@std@@QBE_NABVCCharString@@0@Z, twice), std::_Rb_global<bool>::_M_decrement
//   0x004294E2 (__cdecl), _M_insert 0x00580765 (family J044; both calls reuse v's stack slot for the hidden iterator result).
// Shape: STLport 4.6 _tree.c `insert_unique(const _Value&)` verbatim (`comp = true; while (x) { y = x; comp = cmp(KoV(v), key(x)); x = comp ? left : right; }`,
//   `if (comp) if (j == begin()) return (_M_insert(y, y, v), true); else --j;`, then `cmp(key(j), KoV(v))`).
// Model: less<CCharString>::operator() (0x0042AAB3) and CBasicString<char>::operator< (0x00429950) are given their bodies (noinline, as the landed
//   0x0042AAB3 / 0x00429950 sources) so VC7.1 sees the compare writes no memory and keeps the header pointer in a register across it
//   (declared-only compare: 153v166 on the J045 representative).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
template <class C> class CBasicString {
public:
    C* PData;                                              // +0x00
    __forceinline long Compare(const CBasicString& other) const
    {
        const C* b = other.PData;
        const C* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};
class CCharString {                                        // 4 bytes (Ego_r)
public:
    CBasicString<char>* PData;                             // +0x0
    __forceinline bool operator<(const CCharString& other) const
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
#define CTCHeroStats CTCHeroStatsPdb
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCHeroStats : public CTCHeroStatsPdb {              // engine/CTCHeroStats.h is data-only: the nested record is added here
public:
    struct MirroredCriminalStatus {                        // Ego_r.pdb, 0x44 bytes (struct per the PDB's UMirroredCriminalStatus@CTCHeroStats@@)
        long  Crimes[10];                                  // +0x00
        long  LastTimeSeenByGuard;                         // +0x28
        long  LastTimeExited;                              // +0x2C
        bool  ExitedUsingTeleporter;                       // +0x30
        float EntrancePosX;                                // +0x34
        float EntrancePosY;                                // +0x38
        float EntrancePosZ;                                // +0x3C
        float EntranceAngle;                               // +0x40
    };
};
namespace std {
template <class _Tp> struct less {
    __declspec(noinline) bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }   // 0x0042AAB3 for CCharString (body visible: it writes no memory)
};
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);   // 0x0042971D
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);                // 0x004292C0
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);                // 0x004294E2
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self& operator--() { _M_decrement(); return *this; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    iterator begin() { return iterator(_M_leftmost()); }
    unsigned int size() const { return _M_node_count; }
protected:
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00580765
public:
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
{
    _Link_type __y = _M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
typedef pair<const CCharString, CTCHeroStats::MirroredCriminalStatus > _MapValue;
typedef _Rb_tree<CCharString, _MapValue, _Select1st<_MapValue>, less<CCharString>, allocator<_MapValue> > _MapTree;
template pair<_MapTree::iterator, bool> _MapTree::insert_unique(const _MapValue&);
} // namespace std
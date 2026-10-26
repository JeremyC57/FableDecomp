// std::_Rb_tree::_M_lower_bound(const CCharString& k) const -- (map<CCharString, CTCHeroStats::MirroredCriminalStatus >) STLport lower bound: the first node whose key does not sort before k, the header when none; the key compare is the out-of-line less<CCharString>
// Leaf: ?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@UMirroredCriminalStatus@CTCHeroStats@@@std@@@3@@std@@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057FE4A, 56 bytes, thiscall `ret 4`, the node pointer in eax (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::_Rb_tree<CCharString, pair<const CCharString, CTCHeroStats::MirroredCriminalStatus>, _Select1st<...>, less<CCharString>, allocator<...> >::_M_lower_bound(const CCharString&) const; evidence: its callers are the map's operator[] 0x005812DF (0x005812EF; called by CTCHeroStats::UpdateOutstandingCrimes / GetOutstandingFineAndFramesLeft / GetCrimeReport / OnSerialise and CTCVillage::UpdateMirroredCrimeData, the landed callers naming it map<CCharString, MirroredCriminalStatus>::operator[]; its T() is the 0x44-byte zeroing constructor 0x0057A5DD) and the lower_bound wrappers 0x00580636 / 0x00580A50; Ego_r.pdb CTCHeroStats::OutstandingCrimes is map<CCharString, CTCHeroStats::MirroredCriminalStatus> (68-byte value)
// Label: the manifest's "CCharString_RBTreeFind" is a descriptive placeholder (family J042: five byte-identical lower bounds over CCharString keys, each its
//   own COMDAT; retail has no identical-code folding, so each address is one tree type).
// Callee: std::less<CCharString>::operator() 0x0042AAB3 (??R?$less@VCCharString@@@std@@QBE_NABVCCharString@@0@Z; one COMDAT kept by the linker for every TU).
// Shape: STLport 4.6 `y = header; x = root; while (x) if (!comp(key(x), k)) y = x, x = left(x); else x = right(x); return y;`
//   with the functor called through ebx = &_M_key_compare (+8); key at node +0x10.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
class CCharString {                                        // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
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
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;   // out of line: 0x0042AAB3 for CCharString
};
struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Alloc> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                            // +0x04
    _Compare     _M_key_compare;                           // +0x08
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    static _Link_type _S_left(_Base_ptr __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Base_ptr __x) { return (_Link_type)__x->_M_right; }
    static _Value& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(__x)); }
    _Base_ptr _M_lower_bound(const _Key& __k) const;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;              // last node which is not less than __k
    _Link_type __x = _M_root();                            // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}
typedef pair<const CCharString, CTCHeroStats::MirroredCriminalStatus > _MapValue;
typedef _Rb_tree<CCharString, _MapValue, _Select1st<_MapValue>, less<CCharString>, allocator<_MapValue> > _MapTree;
template _Rb_tree_node_base* _MapTree::_M_lower_bound(const CCharString&) const;
} // namespace std
// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport: hang a new node for v under y (left when y is the header, x is set or v's key is less than y's, unless w forces right), fix the leftmost/rightmost links, rebalance, count it
// Retail: Fable.exe (TLC, VC7.1) @ 0x004759AD, 164 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, v, w).
// Identity: the catalog's "RBTree_InsertOrUpdateScript" is behavioural. Same map<CCharString, long> tree as _M_find
//   @ 0x00475861; called by insert_unique @ 0x004758E3.
// Callees: _Rb_tree::_M_create_node @ 0x0046ECE7 (one call per branch), CBasicString<char> compare @ 0x00411570,
//   _Rb_global<bool>::_Rebalance(x, root) @ 0x0042971D (__cdecl, root by reference).
// Model: STLport 4.6 `_M_insert` verbatim (the y == header / w == 0 / x != 0 / key-compare chain), less<CCharString> and
//   CCharString's operator< inlined; the iterator (user copy constructor) is returned through the hidden pointer.
// Shape: size TU (#pragma optimize("s", on)); the compare's body is visible to the TU as in retail.
// CBasicString<char>'s character compare @ 0x00411570 (landed body), visible to this TU as in retail's header so VC knows it
// writes no memory (the key's block stays hoisted in a register across the calls); never inlined, as retail.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert of
// map<CCharString, long, less<CCharString>, allocator<..> > (Ego_r.pdb, e.g. CWorldMap::LoadedScriptedMapBrushes, CProfileManager::
// ProfileNameToID; descriptive-by-library). The local CCharString model stays (pre-existing). The landed callers still bind
// local-std spellings (insert_unique 0x004758E3: the three-argument _Rb_tree; _M_create_node 0x0046ECE7): NF50 NOTES.
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@J@2@0@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#pragma optimize("s", on)

// CBasicString<char>'s character compare @ 0x00411570 (landed body), visible to this TU as in retail's header so VC knows it
// writes no memory (the key's block stays hoisted in a register across the calls); never inlined, as retail.
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CBasicStringChar {                           // CBasicString<char> (PDB 13 bytes; only the text pointer is read here)
    char* PData;                                    // +0x00
    FABLE_FORCEINLINE bool operator<(const CBasicStringChar& other) const { return Compare_00411570(PData, other.PData) < 0; }
};
class CCharStringData { public:                            // the shared string block (PDB 17 bytes: Data, then NoRefs at +0x0D)
    CBasicStringChar Data;                          // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    FABLE_FORCEINLINE bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};

// less<CCharString>::operator() is expanded in place in retail (0x004759AD: the pointer tests, then `call 0x00411570`); under this
// size TU the header's plain inline body is called out of line, so its explicit specialization is force-inlined here (same symbol).
template<> FABLE_FORCEINLINE bool std::less<CCharString>::operator()(const CCharString& x, const CCharString& y) const { return x < y; }

namespace std {
template _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >, less<CCharString>, allocator<pair<const CCharString, long> > >::iterator _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >, less<CCharString>, allocator<pair<const CCharString, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const CCharString, long>&, _Rb_tree_node_base*);
}

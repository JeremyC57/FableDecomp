// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CWideString, CChecksumCacheFile::CEntry> node insert: allocate a node, copy the pair in, link it left or right of y (keeping leftmost/rightmost), rebalance and count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61270, 233 bytes (+7 int3), `ret 14h` (hidden iterator result + four
//   arguments).
// Callees: CWideString::operator< @ 0x0099AF70 (less<CWideString> inline), malloc @ 0x00BFEA0E (0x34-byte node),
//   CWideString::CWideString(const CWideString&) @ 0x0099B720 (the 0x20-byte CEntry is a `rep movsd` copy),
//   _Rb_global<bool>::_Rebalance @ 0x0042971D.
// Label: the manifest's `ListNode_Insert` is a guess; STLport 4.6 `_Rb_tree::_M_insert`, the same body as the landed
//   CPendingObjectSortKey _M_insert @ 0x00990830.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, as 0x00990830): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert of
// Ego_r.pdb CChecksumCacheFile::Entries = map<CWideString, CChecksumCacheFile::CEntry, less<CWideString>, allocator<..> >
// (descriptive-by-library). The local CWideString / CChecksumCacheFile models stay (pre-existing; fable_string.h has no
// CWideString::operator< and is a root header that is not overlaid). The landed caller 0x00A61360 still binds its local-std
// spelling (V?$pair, one-argument iterator): NF50 NOTES.
// Leaf: ?_M_insert@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCWideString@@UCEntry@CChecksumCacheFile@@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CChecksumCacheFile {
public:
    struct CEntry {                                          // 0x20 bytes, trivially copyable (checksum + CDateAndTime)
    public:
        unsigned long Data[8];
    };
};

namespace std {
template _Rb_tree<CWideString, pair<const CWideString, CChecksumCacheFile::CEntry>, _Select1st<pair<const CWideString, CChecksumCacheFile::CEntry> >, less<CWideString>, allocator<pair<const CWideString, CChecksumCacheFile::CEntry> > >::iterator _Rb_tree<CWideString, pair<const CWideString, CChecksumCacheFile::CEntry>, _Select1st<pair<const CWideString, CChecksumCacheFile::CEntry> >, less<CWideString>, allocator<pair<const CWideString, CChecksumCacheFile::CEntry> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const CWideString, CChecksumCacheFile::CEntry>&, _Rb_tree_node_base*);
}

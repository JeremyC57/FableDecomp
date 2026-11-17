// std::_Rb_tree<CPendingObjectSortKey>::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport node insert for a device-queue map: allocate a node, copy the pair in, link it left or right of y (keeping leftmost/rightmost), rebalance and count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00990830, length 208 bytes (201 + int3 padding), `ret 14h` (hidden iterator
// result + four arguments).
// Callees: less<CThreadedFileManager::CPendingObjectSortKey>::operator() const @ 0x0098F570 (thiscall on the tree's
// _M_key_compare +0x08, not inlined), malloc @ 0x00BFEA0E (the 0x18-byte node: 0x10 links + the 8-byte pair),
// _Rb_global<bool>::_Rebalance(node, root&) @ 0x0042971D (__cdecl; the manifest's "CMemoryAllocatorVariableSize::
// BalanceTree" is propagated; landed as Rbglobalbool_Rebalance_0042971d).
// Callers: nine sites @ 0x009909D8 .. 0x00990FE2 (the CPendingObjectMap inserts of CThreadedFileManager's device queues).
// Identity: FableWin.pdb CThreadedFileManager::CPendingObjectMap::Objects = map<CPendingObjectSortKey, CFileObject*>;
// CPendingObjectSortKey is 4 bytes {CFileObject* Object}; the compare at 0x0098F570 orders by the file's physical sort
// key, then the file position, then the object address. The manifest's "RBTree_Insert" is descriptive.
// Shape: STLport 4.6 _M_insert -- `y == header || (w == 0 && (x != 0 || comp(key(v), key(y))))`; _M_create_node
// (malloc + placement-new copy of the trivially copyable pair under its null test) duplicated in both arms; the iterator
// returned through the hidden pointer.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Select1st@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@U?$less@VCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CThreadedFileManager {                    // no shared header; FableWin.pdb nested types used by the tree
public:
    class CFileObject;
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;                    // +0x00
    };
};
// less<CPendingObjectSortKey>::operator() is out of line at 0x0098F570 (called on the tree's _M_key_compare +0x08).
template<> bool std::less<CThreadedFileManager::CPendingObjectSortKey>::operator()(const CThreadedFileManager::CPendingObjectSortKey&, const CThreadedFileManager::CPendingObjectSortKey&) const;
namespace std {
template _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>, _Select1st<pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> >, less<CThreadedFileManager::CPendingObjectSortKey>, allocator<pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> > >::iterator _Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>, _Select1st<pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> >, less<CThreadedFileManager::CPendingObjectSortKey>, allocator<pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>&, _Rb_tree_node_base*);
}

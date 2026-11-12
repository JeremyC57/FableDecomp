// std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*>, std::_Select1st<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> >, std::less<CThreadedFileManager::CPendingObjectSortKey>, std::allocator<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> > >::_Rb_tree(const std::less<CThreadedFileManager::CPendingObjectSortKey>&, const std::allocator<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@VCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@U?$_Select1st@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@U?$less@VCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@4@@std@@QAE@ABU?$less@VCPendingObjectSortKey@CThreadedFileManager@@@1@ABV?$allocator@U?$pair@$$CBVCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009904B0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*> (agentFAM07 proved this tree for 0x00990B90 map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>::map, RENAMES 2026-11-11): CThreadedFileManager's pending-object map (Ego_r.pdb CPendingObjectMap::Objects, map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>): the landed ~CThreadedFileManager 0x0098E690 calls this tree's _M_erase 0x0098FD40 and binds it with exactly these types.
//   This _Rb_tree constructor 0x009904B0 is the COMDAT right before the tree's ~_Rb_tree 0x009904F0, which calls that _M_erase 0x0098FD40.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_0098fd40,CUnprovenValue_0098fd40,CUnprovenKeyOfValue_0098fd40,std::less<CUnprovenKey_0098fd40>,std::allocator<CUnprovenValue_0098fd40> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_0098fd40@@UCUnprovenValue_0098fd40@@UCUnprovenKeyOfValue_0098fd40@@U?$less@UCUnprovenKey_0098fd40@@@std@@V?$allocator@UCUnprovenValue_0098fd40@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_0098fd40@@@1@ABV?$allocator@UCUnprovenValue_0098fd40@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CThreadedFileManager {
public:
    class CFileObject;
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;                      // +0x00 (Ego_r.pdb, sizeof 4; no shared header for CThreadedFileManager)
    };
};
#include "stlport/fable_map.h"

template std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*>, std::_Select1st<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> >, std::less<CThreadedFileManager::CPendingObjectSortKey>, std::allocator<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> > >::_Rb_tree(const std::less<CThreadedFileManager::CPendingObjectSortKey>&, const std::allocator<std::pair<CThreadedFileManager::CPendingObjectSortKey const, CThreadedFileManager::CFileObject*> >&);

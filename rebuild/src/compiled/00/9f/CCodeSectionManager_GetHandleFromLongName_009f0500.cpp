// CCodeSectionManager::GetHandleFromLongName(CCharString longName) -- the section handle registered under this long name, or 0xFFFFFFFF when none is
// Leaf: ?GetHandleFromLongName@CCodeSectionManager@@QAEKVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0500, 50 bytes (`ret 4`; the callee destroys the by-value string).
// Label: the manifest's "NQuestManager::GetAnimationEntry" is propagated. Callers: LoadSectionLongName 0x004AF000 and
// UnloadSectionLongName 0x004AF030 (`LoadSection(GetHandleFromLongName(name))`) and two quest-manager sites (0x004B3CA3,
// 0x004B3DFC); FableWin.pdb lists ?GetHandleFromLongName@CCodeSectionManager@@QAEKVCCharString@@@Z.
// Callees: map<CCharString, unsigned long>::find @ 0x009D2DD0 (returns the node or the header), ~CCharString @ 0x0099EAE0.
// Layout: as the landed CCodeSectionManager::RegisterSection 0x009F05A0 (12-byte retail STLport maps): LongNameToHandle +0x10;
// the node's mapped handle at +0x14 (links +0..+0xc, key +0x10).
// Shape: the header is read AFTER the find (`cmp eax,[esi]`); the by-value parameter's destructor is inlined into both return
// paths (the miss path returns `or eax,-1`, the hit path parks the handle in esi across the destructor).
// Naming (NF180, 2026-11-20, q4394): the local CCharString / CRetailMap stand-ins (find bound to 0x009D2DD0 as
//   ?find@?$CRetailMap@VCCharString@@K@@QAEPAUNode@1@ABVCCharString@@@Z) replaced by fable_string.h and the shared STLport map
//   (stlport/fable_map.h, _M_find out of line: FABLE_RETAIL_STLP_TREE_MFIND_DECL), so the lookup binds the spelling the self row
//   0x009D2DD0 defines. CCodeSectionManager has no shared header: the row-local model keeps its token, its maps now typed
//   std::map<CCharString, unsigned long> (Ego_r.pdb ShortNameToHandle / LongNameToHandle). Leaf line added (FableWin.pdb spelling).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "fable_string.h"
#include "stlport/fable_map.h"

class CCodeSectionManager {
public:
    virtual void v00();
    std::map<CCharString, unsigned long> ShortNameToHandle;   // +0x04
    std::map<CCharString, unsigned long> LongNameToHandle;    // +0x10 (_M_find 0x009D2DD0)
    unsigned long GetHandleFromLongName(CCharString longName);
};

unsigned long CCodeSectionManager::GetHandleFromLongName(CCharString longName)
{
    std::map<CCharString, unsigned long>::iterator it = LongNameToHandle.find(longName);
    if (it == LongNameToHandle.end())
        return 0xFFFFFFFF;
    return (*it).second;
}

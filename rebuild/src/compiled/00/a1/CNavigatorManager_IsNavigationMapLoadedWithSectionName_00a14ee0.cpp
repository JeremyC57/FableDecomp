// CNavigatorManager::IsNavigationMapLoadedWithSectionName(long id, const CCharString* name) -- find the navigation map with the id (+0x10 of the nodes on the list at +0x24) and compare its section name (+0x24) with the given one: the CCharString equality inlined (same data block, a null block, equal lengths, then Compare @ 0x00411570 == 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14EE0, length 96 bytes.
// Leaf: ?IsNavigationMapLoadedWithSectionName@CNavigatorManager@@QAE_NJPBVCCharString@@@Z
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`). Shape: no frame, the id search loop aligned, the `== 0` as `neg ; sbb ; inc`, `ret 8`.
#include "fable_compiler.h"
// Name fix (NF199, 2026-11-20, q4451): the local CCharString / CCharStringData model, its Compare_00411570 stand-in for 0x00411570 and
//   the free operator== are replaced by the shared fable_string.h (FABLE_RETAIL_CCHARSTRING; the plain inline
//   operators match, _FORCEINLINE_EQ is not needed): CCharString::operator== -> CBasicString<char>::operator== (length, then Compare) binds the
//   protected static ?Compare@?$CBasicString@D@@KIJPBD0@Z (Ego_r.pdb), the self row 0x00411570's Leaf.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
struct SMapNode_00a14ee0 {
    SMapNode_00a14ee0* m_pNext;                    // +0x00
    SMapNode_00a14ee0* m_pPrev;                    // +0x04
    unsigned char      _pad08[8];
    long               m_id;                       // +0x10
    unsigned char      _pad14[0x10];
    CCharString        m_sectionName;              // +0x24
};

class CNavigatorManager { public:
    unsigned char      _pad00[0x24];
    SMapNode_00a14ee0* m_pMaps;                    // +0x24: the sentinel
    bool IsNavigationMapLoadedWithSectionName(long id, const CCharString* name);
};

bool CNavigatorManager::IsNavigationMapLoadedWithSectionName(long id, const CCharString* name)
{
    for (SMapNode_00a14ee0* node = m_pMaps->m_pNext; node != m_pMaps; node = node->m_pNext) {
        if (node->m_id == id)
            return node->m_sectionName == *name;
    }
    return false;
}

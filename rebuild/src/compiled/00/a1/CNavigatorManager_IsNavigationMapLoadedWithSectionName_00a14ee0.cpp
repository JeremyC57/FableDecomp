// CNavigatorManager::IsNavigationMapLoadedWithSectionName(long id, const CCharString* name) -- find the navigation map with the id (+0x10 of the nodes on the list at +0x24) and compare its section name (+0x24) with the given one: the CCharString equality inlined (same data block, a null block, equal lengths, then Compare @ 0x00411570 == 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14EE0, length 96 bytes.
// Leaf: IsNavigationMapLoadedWithSectionName
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`). Shape: no frame, the id search loop aligned, the `== 0` as `neg ; sbb ; inc`, `ret 8`.
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
};
int __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pStringData;
    CCharStringData* b = sb.m_pStringData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}

struct SMapNode_00a14ee0 {
    SMapNode_00a14ee0* m_pNext;                    // +0x00
    SMapNode_00a14ee0* m_pPrev;                    // +0x04
    unsigned char      _pad08[8];
    long               m_id;                       // +0x10
    unsigned char      _pad14[0x10];
    CCharString        m_sectionName;              // +0x24
};

struct CNavigatorManager {
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
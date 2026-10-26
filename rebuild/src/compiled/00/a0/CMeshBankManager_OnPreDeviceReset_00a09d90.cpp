// CMeshBankManager::OnPreDeviceReset() -- walk the bank list at +4 (a sentinel node; the bank at +0xc of each node) and free every bank's device-specific resources (0x00A24000)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A09D90, length 48 bytes.
// Leaf: ?OnPreDeviceReset@CMeshBankManager@@UAEXXZ
// Callees: CMeshDataBank::FreeDeviceSpecificResources @ 0x00A24000. Shape: `this` in edi, the node in esi, the sentinel re-read for the loop test, the loop head aligned, `ret`.
class CMeshDataBank { public:
    void FreeDeviceSpecificResources();            // 0x00A24000
};
struct SBankNode_00a09d90 {
    SBankNode_00a09d90* m_pNext;                   // +0x00
    SBankNode_00a09d90* m_pPrev;                   // +0x04
    long                m_8;
    CMeshDataBank*      m_pBank;                   // +0x0c
};

class CMeshBankManager { public:
    virtual void v000();
    SBankNode_00a09d90* m_pBanks;                  // +0x04: the list sentinel
    virtual void OnPreDeviceReset();
};

void CMeshBankManager::OnPreDeviceReset()
{
    for (SBankNode_00a09d90* node = m_pBanks->m_pNext; node != m_pBanks; node = node->m_pNext)
        node->m_pBank->FreeDeviceSpecificResources();
}
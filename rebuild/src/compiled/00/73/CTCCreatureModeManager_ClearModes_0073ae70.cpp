// CTCCreatureModeManager::ClearModes() -- tell the front mode (when any) it is leaving (virtual slot 6), clear the counted-pointer list at +0xc, then select mode 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AE70, length 43 bytes.
// Callees: the mode's virtual slot 6 (+0x18), CRetailList_CountedPointer::clear @ 0x0073E6A0 (on the list at +0xc; landed), AddMode @ 0x0073AE10 (on `this`, 0).
// Shape: the sentinel test is `cmp [eax],eax` on the head node, the first node's counted pointer object sits at node+8, the list address is formed with `lea esi`
// before the test and reused for the clear. The chunk label "CEnginePrimitiveRenderer::ClearAllChildPrimitives" is propagated (the 0x0073xxxx module is game code).
struct CCreatureMode_0073ae70 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnLeave();                        // slot 6 (+0x18)
};

struct SModeNode {
    SModeNode*               m_pNext;
    SModeNode*               m_pPrev;
    CCreatureMode_0073ae70*  m_pMode;              // +0x08 (the counted pointer's object word)
    void*                    m_pCount;             // +0x0c
};

struct CRetailList_CountedPointer {
    SModeNode* m_pHead;                            // the sentinel node
    void clear();                                  // 0x0073E6A0
};

class CTCCreatureModeManager { public:
    unsigned char              _pad[0xc];
    CRetailList_CountedPointer m_modes;            // +0x0c
    void ClearModes();
public:
    bool AddMode(unsigned long);   // 0x0073AE10
};

void CTCCreatureModeManager::ClearModes()
{
    if (m_modes.m_pHead->m_pNext != m_modes.m_pHead)
        m_modes.m_pHead->m_pNext->m_pMode->OnLeave();
    m_modes.clear();
    AddMode(0);
}
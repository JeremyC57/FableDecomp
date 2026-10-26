// CTCCreatureModeManager::IsModeActive(unsigned long type) const -- whether any mode on the counted-pointer mode list at +0xC reports the given type (virtual slot 1)
// Leaf: IsModeActive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A8A0, length 54 bytes.
// Identity: the catalog label "CPlayer::HasMode" is propagated. Ego_r.pdb has
// `?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z` immediately before AllocateMode, as here (the
// next function, 0x0073A8E0, is CTCCreatureModeManager::AllocateMode in the manifest); PDB
// CTCCreatureModeManager::PModes is the list<CCountedPointer<CModeBase>> at +0xC.
// Callees: CModeBase's virtual type getter (slot 1, `call [eax+4]`).
// Shape: an inlined find over the list with the head (end) cached in edi for the loop, the type in
// ebx (pushed only on the non-empty path), and `found != PModes.end()` re-reading the head after
// the loop (`mov ecx,[ebp+0Ch]`): the comparison is written against the member, not the cached
// end; the bool lands as `xor eax,eax ; setne al`.
struct CModeBase {
    virtual void v00();
    virtual unsigned long GetType() const;         // slot 1
};

struct SModeNode {                                 // list node: next, prev, CCountedPointer<CModeBase>
    SModeNode*     m_pNext;                        // +0x00
    SModeNode*     m_pPrev;                        // +0x04
    CModeBase*     m_pMode;                        // +0x08 (CCountedPointer::Data)
    void*          m_pModeInfo;                    // +0x0c (CCountedPointer::Info)
};

class CTCCreatureModeManager { public:
    unsigned char _pad00[0xc];
    SModeNode*    PModes;                          // +0x0c list head (the sentinel)
    bool IsModeActive(unsigned long type) const;
};

bool CTCCreatureModeManager::IsModeActive(unsigned long type) const
{
    SModeNode* end = PModes;
    SModeNode* node = end->m_pNext;
    for (; node != end; node = node->m_pNext) {
        if (node->m_pMode->GetType() == type)
            break;
    }
    return node != PModes;
}
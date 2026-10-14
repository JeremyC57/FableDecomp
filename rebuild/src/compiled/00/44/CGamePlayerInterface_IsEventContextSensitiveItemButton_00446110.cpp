// CGamePlayerInterface::IsEventContextSensitiveItemButton(const CGameEvent& event, unsigned long* outIndex) -- finds the context-sensitive item button that matches the event (virtual slot 3 on each counted input type), returning its index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00446110, length 93 bytes.
// Callees: CInputTypeBase virtual slot 3 (+0xc), `bool (const CGameEvent&)`.
// Layout: ContextSensitiveItemButtons is a retail 12-byte std::vector<CCountedPointer<const CInputTypeBase>> whose begin/end sit at
// +0x7d0/+0x7d4 (the PDB puts it at +0x7dc with 16-byte vectors; engine/CGamePlayerInterface.h follows the PDB).
// Shape: size = (end - begin) >> 3 (signed pointer difference) re-read every iteration, compared unsigned against the index;
// the element's data pointer is `[begin+i*8]`; the event reference is hoisted into ebx; `mov al,1` / `xor al,al` = bool.
struct CGameEvent;
struct SCountBlock;

struct CInputTypeBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool IsButtonEvent(const CGameEvent& event);    // slot 3
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
};

template <class T>
struct CRetailVector {                              // retail std::vector: begin / end / end-of-storage
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

struct CGamePlayerInterface {
    unsigned char _pad[0x7d0];
    CRetailVector< CCountedPointer<CInputTypeBase> > ContextSensitiveItemButtons;   // +0x7d0
    bool IsEventContextSensitiveItemButton(const CGameEvent& event, unsigned long* outIndex);
};

bool CGamePlayerInterface::IsEventContextSensitiveItemButton(const CGameEvent& event, unsigned long* outIndex)
{
    for (unsigned long i = 0; i < ContextSensitiveItemButtons.Size(); ++i) {
        if (ContextSensitiveItemButtons[i].m_pObject->IsButtonEvent(event)) {
            *outIndex = i;
            return true;
        }
    }
    return false;
}
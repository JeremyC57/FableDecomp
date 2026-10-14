// NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank) -- share the bank's object and count block into the manager's counted pointer at +0x10 (0x00419134), the by-value argument then destroyed in its slot (0x00419108)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A9B7, length 28 bytes.
// Callees: CCountedPointer::ShareData(object, block) @ 0x00419134 (the inlined assignment), CCountedPointer::~CCountedPointer @ 0x00419108 (on the argument slot; the manifest label "_Dest_val<...>" is propagated). Shape: the two words pushed from the argument slots, `add ecx,10h`, the destructor on `[esp+4]`, `ret 8`. The retail bank pointer sits at +0x10 (engine/CManager.h has it at +0x20): modelled locally.
#pragma optimize("s", on)
struct CGraphicDataBank;
struct CCPPointerInfo;
struct CCountedPointer_CGraphicDataBank {          // {object, count block}
    CGraphicDataBank* m_pObject;                   // +0x00
    CCPPointerInfo*   m_pInfo;                     // +0x04
    CCountedPointer_CGraphicDataBank(const CCountedPointer_CGraphicDataBank& other);   // the by-value argument is built by its copy constructor
    ~CCountedPointer_CGraphicDataBank();           // 0x00419108
    void ShareData(CGraphicDataBank* object, CCPPointerInfo* info);   // 0x00419134
    CCountedPointer_CGraphicDataBank& operator=(const CCountedPointer_CGraphicDataBank& other)
    {
        ShareData(other.m_pObject, other.m_pInfo);
        return *this;
    }
};

namespace NUISystem {
struct CManager {
    unsigned char                    _pad[0x10];
    CCountedPointer_CGraphicDataBank m_graphicsBank;   // +0x10
    void SetGraphicsBank(CCountedPointer_CGraphicDataBank bank);
};
}

void NUISystem::CManager::SetGraphicsBank(CCountedPointer_CGraphicDataBank bank)
{
    m_graphicsBank = bank;
}
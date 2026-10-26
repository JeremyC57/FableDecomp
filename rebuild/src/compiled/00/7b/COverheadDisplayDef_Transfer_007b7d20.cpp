// COverheadDisplayDef::Transfer(CPersistContext&) -- persist the three overhead-display switches (damage, repair, renown)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B7D20, 184 bytes (+ int3 padding; virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC 0x00404500 (x3, the empty key "" 0x0122D70E), CPersistTraits<bool>::TransferBinaryIn
//   0x00403EB0, CDataOutputStream::WriteEBOOL 0x00993E30.
// Shape: three inline CPersistContext::Transfer<bool> calls of the landed CActivateQuestDef::Transfer 0x007B5740 template
//   (member address formed before its CRC call, the `sub eax,2 ; je ; dec eax ; jne` mode switch, the empty traits temporary
//   in the dead argument slot); the last member reuses `this` (`add edi,27h`) and gets its own epilogue per arm. `this` is the
//   engine header layout (DisplayDamage +0x25, DisplayRepair +0x26, DisplayRenown +0x27).
#include "engine/COverheadDisplayDef.h"

class CMemoryDataInputStream;
class CDataOutputStream { public:
public:
    void WriteEBOOL(bool);   // 0x00993E30
};
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteEBOOL(value); }
};
class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};
struct COverheadDisplayDef_Transfer : COverheadDisplayDef {
    void Transfer(CPersistContext& context);
};

void COverheadDisplayDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", DisplayDamage);
    context.Transfer("", DisplayRepair);
    context.Transfer("", DisplayRenown);
}
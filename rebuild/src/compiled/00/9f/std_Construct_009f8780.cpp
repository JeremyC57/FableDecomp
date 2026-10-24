// std::_Construct(CGraphicBankManager::CGraphicDataBankInfo*, const CGraphicBankManager::CGraphicDataBankInfo&) -- placement copy of a graphic-bank record: the 0x2C-byte init block (rep movsd), the bank handle string and a counted bank pointer
// Retail: Fable.exe @ 0x009F8780, 59 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N009f8780 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CCPPointerInfo { long RefCount; };
template <class T> struct CCountedPointer {      // shared pointer: object + shared count block
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct CGraphicDataBankInit { unsigned long Words[11]; };   // 0x2C bytes, trivially copied
struct CGraphicDataBank;
struct CGraphicDataBankInfo {                    // CGraphicBankManager::CGraphicDataBankInfo (PDB, sizeof 0x38)
    CGraphicDataBankInit              Init;             // +0x00
    CCharString                       BankHandle;       // +0x2C
    CCountedPointer<CGraphicDataBank> GraphicDataBank;  // +0x30
    __forceinline CGraphicDataBankInfo(const CGraphicDataBankInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : Init(o.Init), BankHandle(o.BankHandle), GraphicDataBank(o.GraphicDataBank) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CGraphicDataBankInfo* p, const CGraphicDataBankInfo& value)
{
    if (p != 0)
        new (p) CGraphicDataBankInfo(value);
}
}
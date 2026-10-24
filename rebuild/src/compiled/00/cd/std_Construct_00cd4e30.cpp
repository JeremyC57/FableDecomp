// std::_Construct(CSymbolMap::CSymbolInfo*, const CSymbolMap::CSymbolInfo&) -- placement copy of a symbol entry: two CCharStrings and a long (sizeof 0xC)
// Retail: Fable.exe @ 0x00CD4E30, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00cd4e30 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CSymbolInfo {                             // CSymbolMap::CSymbolInfo (PDB, sizeof 0xC)
    CCharString GroupName;                       // +0x00
    CCharString EntryName;                       // +0x04
    long        Val;                             // +0x08
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CSymbolInfo* p, const CSymbolInfo& value)
{
    if (p != 0)
        new (p) CSymbolInfo(value);
}
}
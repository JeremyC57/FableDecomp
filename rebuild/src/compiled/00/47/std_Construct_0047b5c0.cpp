// std::_Construct(CHeroLogBookEntry*, const CHeroLogBookEntry&) -- placement copy of a hero log-book entry: name, abbreviated name and content wide strings (catalog label pair<CCharString,CPersistInfo> is propagated; three CWideStrings = CHeroLogBookEntry)
// Retail: Fable.exe @ 0x0047B5C0, 43 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CWideString copy ctor @ 0x0099B720 (x3).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0047b5c0 {
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString(const CWideString& other);       // 0x0099B720
};
struct CHeroLogBookEntry {                       // PDB sizeof 0xC
    CWideString Name;                            // +0x00
    CWideString AbbreviatedName;                 // +0x04
    CWideString Content;                         // +0x08
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CHeroLogBookEntry* p, const CHeroLogBookEntry& value)
{
    if (p != 0)
        new (p) CHeroLogBookEntry(value);
}
}
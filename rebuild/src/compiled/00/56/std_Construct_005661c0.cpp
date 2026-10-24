// std::_Construct(CTCHeroOnlineScoreboard::CClanMember*, const CTCHeroOnlineScoreboard::CClanMember&) -- placement copy of a clan member: wide name, id, wide description
// Retail: Fable.exe @ 0x005661C0, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CWideString copy ctor @ 0x0099B720 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N005661c0 {
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString(const CWideString& other);       // 0x0099B720
};
struct CClanMember {                             // CTCHeroOnlineScoreboard::CClanMember (PDB, sizeof 0xC)
    CWideString Name;                            // +0x00
    long        MemberID;                        // +0x04
    CWideString Description;                     // +0x08
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CClanMember* p, const CClanMember& value)
{
    if (p != 0)
        new (p) CClanMember(value);
}
}
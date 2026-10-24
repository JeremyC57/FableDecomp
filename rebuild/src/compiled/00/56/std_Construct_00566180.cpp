// std::_Construct(CTCHeroOnlineScoreboard::CClanInfo*, const CTCHeroOnlineScoreboard::CClanInfo&) -- placement copy of a clan record: wide name, id, wide description, home-page string, member count
// Retail: Fable.exe @ 0x00566180, 55 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CWideString copy ctor @ 0x0099B720 (x2), CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00566180 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString(const CWideString& other);       // 0x0099B720
};
struct CClanInfo {                               // CTCHeroOnlineScoreboard::CClanInfo (PDB, sizeof 0x14)
    CWideString Name;                            // +0x00
    long        ClanID;                          // +0x04
    CWideString Description;                     // +0x08
    CCharString HomePageURL;                     // +0x0C
    int         NumMembers;                      // +0x10
    __forceinline CClanInfo(const CClanInfo& o)   // the implicit copy, spelled out so VC7.1 inlines it into _Construct
        : Name(o.Name), ClanID(o.ClanID), Description(o.Description), HomePageURL(o.HomePageURL), NumMembers(o.NumMembers) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CClanInfo* p, const CClanInfo& value)
{
    if (p != 0)
        new (p) CClanInfo(value);
}
}
// std::_Construct(CTCHeroOnlineScoreboard::CActivity*, const CTCHeroOnlineScoreboard::CActivity&) -- placement copy of a scoreboard activity: three CCharStrings, decimal places and a lower-is-better flag
// Retail: Fable.exe @ 0x00565780, 55 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x3).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00565780 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CActivity {                               // CTCHeroOnlineScoreboard::CActivity (PDB, sizeof 0x10)
    CCharString   Name;                          // +0x00
    CCharString   Description;                   // +0x04
    CCharString   UnitOfMeasurement;             // +0x08
    unsigned char NumDecPlacesToShow;            // +0x0C
    bool          LowerIsBetter;                 // +0x0D
    __forceinline CActivity(const CActivity& o) : Name(o.Name), Description(o.Description), UnitOfMeasurement(o.UnitOfMeasurement), NumDecPlacesToShow(o.NumDecPlacesToShow), LowerIsBetter(o.LowerIsBetter) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CActivity* p, const CActivity& value)
{
    if (p != 0)
        new (p) CActivity(value);
}
}
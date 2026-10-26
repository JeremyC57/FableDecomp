// CCharString::GetCRC() const -- CRC-32 of the string's characters (the empty string's when there is no data)
// Leaf: ?GetCRC@CCharString@@QBEKXZ
// Retail: Fable.exe @ 0x00404310, 46 bytes, plain `ret`; Ego_r.pdb ?GetCRC@CCharString@@QBEKXZ. Body:
// CCRC::Calc(0, GetPString(), GetLength()) -- 0x004014A0 is ?Calc@CCRC@@SIKKPBXK@Z (fastcall seed/data, length on the
// stack; landed callers declare it that way, e.g. 0x008C7C00) -- with GetLength (PStringData ? +4 : 0) evaluated first and
// GetPString (PStringData ? +0 : the shared "" at 0x0129AAF4), and the call duplicated into both arms. Catalog name
// CCharString_ComputeCRC32 is descriptive; 15+ game callers (0x0047F150, 0x0048F6DC, 0x00497EF4, ..). A byte-identical
// body at 0x009914B0, beside CCRC::CalcFromPathname, is the CCRC::CalcFromString(const CCharString&) twin.
// Layout: Ego_r.pdb CCharString { CCharStringData* PStringData } with CBasicString<char> { char* PData; ulong StringLength }
// at the head of CCharStringData. No engine/ header for CCharString.
// Shape: /O2; both getters test the null case first (`PStringData == 0 ? .. : ..`), which puts the empty-string arms first.
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
class CCharStringData { public:
    char*         PData;                                 // +0x00 CBasicString<char>::PData
    unsigned long StringLength;                          // +0x04
};
extern const char g_EmptyString[];                       // 0x0129AAF4
class CCharString {
public:
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned long GetCRC() const;
};
unsigned long CCharString::GetCRC() const
{
    return CCRC::Calc(0, GetPString(), GetLength());
}
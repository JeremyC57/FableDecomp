// CUserProfileManager::GetSaveDisplayNameFromFileName(const CWideString&, long) const -- "AutoSave" shows as the autosave display name, a manual save ("Save..." or containing " - Save") as the numbered manual-save display name, anything else as itself
// Leaf: GetSaveDisplayNameFromFileName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00407E10, 437 bytes (448 with padding), hidden result, `ret 0Ch`.
// Callees: CWideString(const wchar_t*) 0x0099B6B0 / (const CWideString&) 0x0099B720 / ~CWideString 0x0099B510,
//   MemCmp_Unsigned16 0x00403C60 (the inline ==), CWideString::ToCharString 0x0099B2C0, ~CCharString 0x0099EAE0,
//   strncmp 0x00BFEAF8, operator+(const wchar_t*, const CWideString&) 0x0099BFF0 (`__fastcall`), CWideString::Contains
//   0x0099AF20, CUserProfileManager::GetAutoSaveDisplayName 0x00406C20 / GetManualSaveDisplayName(long) 0x00406AE0.
//   The empty narrow string 0x0129AAF4.
// Strings: L"AutoSave" 0x0122DE9C, L"Save" 0x0122DE90, L" - " 0x0122E070.
// Shape: the base names come from the inline GetAutoSaveFileName / GetManualSaveBaseFileName (returned temporaries,
//   addressed on the stack); the manual-save test is a named bool over five conditional temporaries (ebx flags
//   1..0x10); the prefix test is CCharString's inline StartsWith (strncmp over the prefix length); the inline wide ==
//   reads both starts first and compares the two signed lengths.
struct StringRep { char* data; long len; };
extern char g_EmptyCharString[1];                       // 0x0129AAF4
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8
struct CCharString {
    StringRep* rep;
    ~CCharString();                                     // 0x0099EAE0
    long GetLength() const
    {
        if (rep == 0)
            return 0;
        return rep->len;
    }
    operator const char*() const
    {
        if (rep == 0)
            return g_EmptyCharString;
        return rep->data;
    }
    __forceinline bool StartsWith(const CCharString& prefix) const { return strncmp(*this, prefix, prefix.GetLength()) == 0; }
};
int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60
struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
    long size() const { return (long)(_M_finish - _M_start); }
};
struct CWideString {
    CWideStringData* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool Contains(const CWideString& text) const;       // 0x0099AF20
    CCharString ToCharString() const;            // 0x0099B2C0
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        const unsigned short* otherStart = other.PStringData->_M_start;
        const unsigned short* start = PStringData->_M_start;
        long length = PStringData->_M_finish - start;
        return length == other.PStringData->_M_finish - otherStart && MemCmp_Unsigned16(start, otherStart, length) == 0;
    }
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);      // 0x0099BFF0

class CUserProfileManager {
public:
    void*       __vftable;                              // +0x00
    long        _unknown04;                             // +0x04
    CWideString ProfileName;                            // +0x08
    CWideString GetManualSaveBaseFileName() const { return (const unsigned short*)L"Save"; }
    CWideString GetAutoSaveFileName() const { return (const unsigned short*)L"AutoSave"; }
    CWideString GetManualSaveDisplayName(long slot) const;   // 0x00406AE0
    CWideString GetAutoSaveDisplayName() const;              // 0x00406C20
    CWideString GetSaveDisplayNameFromFileName(const CWideString& fileName, long slot) const;
};

CWideString CUserProfileManager::GetSaveDisplayNameFromFileName(const CWideString& fileName, long slot) const
{
    if (fileName == GetAutoSaveFileName())
        return GetAutoSaveDisplayName();
    bool manual = fileName.ToCharString().StartsWith(GetManualSaveBaseFileName().ToCharString())
        || fileName.Contains((const unsigned short*)L" - " + GetManualSaveBaseFileName());
    if (manual)
        return GetManualSaveDisplayName(slot);
    return fileName;
}
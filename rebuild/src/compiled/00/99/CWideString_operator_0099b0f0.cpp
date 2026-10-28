// CWideString::operator!=(const CWideString& other) const -- true unless both strings share one data block or hold the same characters (a string with no data block equals only another null string)
// Leaf: ??9CWideString@@QBE_NABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B0F0, 88 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): the 2-byte character count (`sar 1` of the data pointer difference) makes it a CWideString member, and it
//   sits among the landed CWideString members (FindCharPos 0x0099B070, operator!=(const wchar_t*) 0x0099B150, operator!=(wchar_t)
//   0x0099B1D0); the result is the negation of an equality (identical blocks -> false), and the PDBs' three ??9CWideString
//   overloads are (const CWideString&), (const wchar_t*), (wchar_t): this is the CWideString& one, ??9CWideString@@QBE_NABV0@@Z.
// Label correction: the catalog's "CCharString::operator==" is wrong (wide string; inequality).
// Callees: the wide character compare 0x00403C60 (cdecl; landed as MemCmp_Unsigned16 -- STLport's
//   __char_traits_base<wchar_t, wint_t>::compare, called from basic_string's operator==).
// Shape: `return !(*this == other);` over an inline operator== (same block -> true; either block null -> false; else
//   CWideStringData::operator== = the STLport basic_string equality `size() == other.size() && compare(...) == 0`), the
//   negation `xor edx,edx ; test al,al ; sete dl` duplicated on both arms (as the landed operator!=(wchar_t) 0x0099B1D0). Both
//   inline operators need __forceinline: plain inline left CWideStringData::operator== out of line.
// Layout: CWideString is one CWideStringData* (Ego_r); CWideStringData is the STLport basic_string<wchar_t> (start, finish,
//   end of storage: 12 bytes in retail) then NoRefs. Characters are modelled as unsigned short (no /Zc:wchar_t needed here).
int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60

class CWideStringData {
public:
    unsigned short* DataStart;                          // +0x00 basic_string<wchar_t> Data: _M_start
    unsigned short* DataFinish;                         // +0x04 _M_finish
    unsigned short* DataEndOfStorage;                   // +0x08 _M_end_of_storage
    long            NoRefs;                             // +0x0C
    unsigned int size() const { return unsigned int(DataFinish - DataStart); }
    __forceinline bool operator==(const CWideStringData& other) const
    {
        return size() == other.size() && MemCmp_Unsigned16(DataStart, other.DataStart, size()) == 0;
    }
};

class CWideString {
public:
    CWideStringData* PStringData;                       // +0x00
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return *PStringData == *other.PStringData;
    }
    bool operator!=(const CWideString& other) const;
};

bool CWideString::operator!=(const CWideString& other) const
{
    return !(*this == other);
}
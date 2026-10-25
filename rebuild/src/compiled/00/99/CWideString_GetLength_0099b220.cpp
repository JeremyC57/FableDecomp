// CWideString::GetLength() const -- the number of wide characters in the shared string data (0 for a null string):
// the STLport basic_string<wchar_t> size(), `(_M_finish - _M_start) >> 1`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B220, length 17 bytes.
// Name: Ego_r.pdb 0x00B02190 / FableWin.pdb 0x02F66000 ?GetLength@CWideString@@QBEJXZ. The PDB layouts: CWideString holds
//   one CWideStringData* PStringData, and CWideStringData is { basic_string<wchar_t> Data; long NoRefs; } -- a 2-byte
//   element string, hence the `sar eax,1`. CCharString's data is a CBasicString<char> whose StringLength is a stored
//   field (+4), so `?GetLength@CCharString@@QBEJXZ` cannot be this body. Was catalogued `CCharString::GetLength`
//   (chunk 203/204 label fix and agent420 NOTES: called on CChecksumCacheFile::Filename, a CWideString).
// Model: local classes (no engine/CWideString.h); STLport basic_string's three pointers (12 bytes, not the PDB's 28).
// Leaf: ?GetLength@CWideString@@QBEJXZ
#ifndef _WCHAR_T_DEFINED                           // no /Zc:wchar_t: the CRT's typedef (mangles G)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif
namespace std {
template <class _CharT> struct char_traits { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {
public:
    _CharT* _M_start;                              // +0x00
    _CharT* _M_finish;                             // +0x04
    _CharT* _M_end_of_storage;                     // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
};
}

class CWideStringData {
public:
    std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > Data;   // +0x00
    long NoRefs;                                   // +0x0C
};

class CWideString {
public:
    CWideStringData* PStringData;                  // +0x00
    long GetLength() const;
};

long CWideString::GetLength() const
{
    if (PStringData == 0)
        return 0;
    return PStringData->Data.size();
}

// CSymbolMap::GetSymbolsFromGroupName(const CCharString& group, CArray<CSymbolInfo>& symbols) -- append every SLONGMap symbol whose GroupName equals the group to the array; true when at least one was found
// Leaf: GetSymbolsFromGroupName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E5290, length 185 bytes.
//   PDB: ?GetSymbolsFromGroupName@CSymbolMap@@QBE_NABVCCharString@@AAV?$CArray@VCSymbolInfo@CSymbolMap@@@@@Z.
// Label correction: the catalog's "CCombatWheel_TryInsertMatchingString" is CSymbolMap::GetSymbolsFromGroupName: the walk is
//   over the 16-byte pair<CCharString, CSymbolInfo> entries of SLONGMap (+4 / +8), comparing pair.second.GroupName (+4).
// Callees: CCharString Compare @ 0x00411570 (`__fastcall`, inside the inline CCharString::operator==), the CCharString copy
//   constructor @ 0x0099EC30 (twice: the implicit CSymbolInfo copy), STLport vector<CSymbolInfo>::_M_insert_overflow
//   @ 0x009E76F0 (manifest "CCombatWheel::InsertElements" is propagated).
// Model: CSymbolMap {_padding_ +0, SLONGMap +4} as the landed GetSymbolSLONG 0x009E5120; CArray<T> is the retail 12-byte
//   STLport vector. CCharString equality is the landed inline operator== (0x0099E900 model: same block, both non-null and
//   equal length, `Compare(...) == 0` -> `neg ; sbb ; inc`).
// Shape: the `found` bool lives in a frame byte (and `this` is spilled: ebx/ebp/esi/edi hold the pair, the array and the
//   copy pointers); the group reference is re-read each pass, end() re-read through the spilled `this`. push_back is the
//   STLport inline: null-tested placement copy and `add [finish],0Ch`, else `_M_insert_overflow(finish, x, __false_type(),
//   1, true)` with the empty tag homed in the dead `symbols` argument slot (the array pointer lives in ebp).
#include <new>

struct CCharStringData {
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
};

namespace _STL {
struct __false_type { __false_type() {} };

template <class _Tp>
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x009E76F0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class T>
class CArray : public _STL::vector<T> {};

class CSymbolMap {
public:
    class CSymbolInfo {                            // 12 bytes
    public:
        CCharString GroupName;                     // +0x00
        CCharString EntryName;                     // +0x04
        long        Val;                           // +0x08
    };
    struct SSymbolPair {                           // pair<CCharString, CSymbolInfo>, 16 bytes
        CCharString first;
        CSymbolInfo second;
    };
    struct CSymbolVectorMap {
        SSymbolPair* m_pFirst;
        SSymbolPair* m_pLast;
        SSymbolPair* m_pEnd;
    };
    int              _padding_;                    // +0x00
    CSymbolVectorMap SLONGMap;                     // +0x04
    bool GetSymbolsFromGroupName(const CCharString& group, CArray<CSymbolInfo>& symbols) const;
};

bool CSymbolMap::GetSymbolsFromGroupName(const CCharString& group, CArray<CSymbolInfo>& symbols) const
{
    bool found = false;
    for (const SSymbolPair* it = SLONGMap.m_pFirst; it != SLONGMap.m_pLast; ++it) {
        if (it->second.GroupName == group) {
            symbols.push_back(it->second);
            found = true;
        }
    }
    return found;
}
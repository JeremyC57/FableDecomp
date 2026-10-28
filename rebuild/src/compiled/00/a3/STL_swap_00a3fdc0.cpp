// std::swap(CSymbolMap::CSymbolInfo& a, CSymbolMap::CSymbolInfo& b) -- exchange two symbol records (group name, entry name, value) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3FDC0, 123 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> esi), plain `ret`.
// Identity: the 12-byte {CCharString, CCharString, long} element of the /Gr STLport block at 0x00A3FB60.. (landed
//   _Construct<CSymbolMap::CSymbolInfo> 0x00A3FB60 and the {string, string, long} __copy_backward 0x00A3FE60); callers are
//   the out-of-line iter_swap 0x00A40100 (a bare `jmp`) and the partition step 0x00A40EB8. Ego_r.pdb
//   CSymbolMap::CSymbolInfo: GroupName +0, EntryName +4, Val +8.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` with CSymbolInfo's implicit copy constructor and
//   assignment (memberwise: two strings, then the long); the temporary dies EntryName first.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CSymbolMap {
public:
    class CSymbolInfo {                                 // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                          // +0x00
        CCharString EntryName;                          // +0x04
        long        Val;                                // +0x08
    };
};

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<CSymbolMap::CSymbolInfo>(CSymbolMap::CSymbolInfo&, CSymbolMap::CSymbolInfo&);
}
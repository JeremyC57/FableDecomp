// std::__destroy_aux(std::pair<CCharString, CSymbolMap::CSymbolInfo>* first, std::pair<CCharString, CSymbolMap::CSymbolInfo>* last, const __false_type&) -- destroy every record of [first, last)
// Leaf: ??$__destroy_aux@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@@std@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FBCC0, 51 bytes, `__fastcall` (first ecx, last edx, the tag by reference), `ret 4`.
// Identity (proven): called by the landed vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::~vector 0x006FCF10 (and its sibling vector helpers); the pair's CCharString +0 and CSymbolInfo {GroupName +4, EntryName +8, Val +0xC} (Ego_r.pdb).
// Callees: the member destructors (CCharString::~CCharString 0x0099EAE0).
// Shape (family M134, representative 0x00567350): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` with
//   the record's implicit destructor inline (members in reverse order); speed TU (the `lea ebx,[ebx]` loop alignment).
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CSymbolMap {
public:
    class CSymbolInfo {                                   // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                            // +0x0
        CCharString EntryName;                            // +0x4
        long Val;                                         // +0x8
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template void __fastcall std::__destroy_aux<std::pair<CCharString, CSymbolMap::CSymbolInfo>*>(std::pair<CCharString, CSymbolMap::CSymbolInfo>*, std::pair<CCharString, CSymbolMap::CSymbolInfo>*, const std::__false_type&);
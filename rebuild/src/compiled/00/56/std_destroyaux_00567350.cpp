// std::__destroy_aux(CTCHeroOnlineScoreboard::CActivity* first, CTCHeroOnlineScoreboard::CActivity* last, const __false_type&) -- destroy every record of [first, last)
// Leaf: ??$__destroy_aux@PAVCActivity@CTCHeroOnlineScoreboard@@@std@@YIXPAVCActivity@CTCHeroOnlineScoreboard@@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567350, 51 bytes, `__fastcall` (first ecx, last edx, the tag by reference), `ret 4`.
// Identity (probable): 16-byte records with CCharStrings at +0/+4/+8 destroyed (reverse order) in CTCHeroOnlineScoreboard's code (callers: its destructor 0x00562FF0, the activity tree / vector helpers 0x00567BB0..0x00569D90); Ego_r.pdb CTCHeroOnlineScoreboard::CActivity {Name, Description, UnitOfMeasurement, NumDecPlacesToShow, LowerIsBetter} (16 bytes; Ego_r.pdb has vector<CActivity>::_Tidy and ~CActivity).
// Callees: the member destructors (CCharString::~CCharString 0x0099EAE0).
// Shape (family M134, representative 0x00567350): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` with
//   the record's implicit destructor inline (members in reverse order); speed TU (the `lea ebx,[ebx]` loop alignment).
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CTCHeroOnlineScoreboard {
public:
    class CActivity {                                     // Ego_r.pdb, 16 bytes
    public:
        ~CActivity() { }
        CCharString Name;                                 // +0x0
        CCharString Description;                          // +0x4
        CCharString UnitOfMeasurement;                    // +0x8
        unsigned char NumDecPlacesToShow;                 // +0xC
        bool LowerIsBetter;                               // +0xD
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
template void __fastcall std::__destroy_aux<CTCHeroOnlineScoreboard::CActivity*>(CTCHeroOnlineScoreboard::CActivity*, CTCHeroOnlineScoreboard::CActivity*, const std::__false_type&);
// _STL::__uninitialized_fill_n(CActivity* first, unsigned int n, const CActivity& x, const __false_type&) -- placement-copy n copies of a CTCHeroOnlineScoreboard::CActivity (three CCharStrings, a decimal-places byte and a lower-is-better flag) and return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005657C0, length 78 bytes, `ret 8`.
// Callers: 0x0056651A, 0x00569072, 0x00569604 (the scoreboard activity vector's fill/insert paths).
// Callees: CCharString copy constructor @ 0x0099EC30 (x3 per element).
// Label: catalog "std::__init_range_fixed_struct" is descriptive; the body is STLport's non-POD __uninitialized_fill_n.
// Element: CTCHeroOnlineScoreboard::CActivity (PDB sizeof 0x10; the same model as the landed _Construct 0x00565780):
//   Name +0, Description +4, UnitOfMeasurement +8 (CCharString), NumDecPlacesToShow +0xC, LowerIsBetter +0xD.
// Shape: STLport 4.x `_uninitialized.h` verbatim (`_ForwardIter __cur = __first; for ( ; __n > 0; --__n, ++__cur)
//   _Construct(&*__cur, __x); return __cur;`, exceptions off) in a /Gr algorithm TU: first in ecx, the unsigned count in
//   edx (`test edx,edx ; jbe`), &x and the unused tag on the stack (`ret 8`). _Construct is the null-tested placement new
//   of the implicit memberwise copy constructor.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                               // the shared string block
    CCharString(const CCharString& other);           // 0x0099EC30
};

class CTCHeroOnlineScoreboard {
public:
    class CActivity {                                // PDB sizeof 0x10
    public:
        CCharString   Name;                          // +0x00
        CCharString   Description;                   // +0x04
        CCharString   UnitOfMeasurement;             // +0x08
        unsigned char NumDecPlacesToShow;            // +0x0C
        bool          LowerIsBetter;                 // +0x0D
    };
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

typedef CTCHeroOnlineScoreboard::CActivity TActivity;
template TActivity* __fastcall _STL::__uninitialized_fill_n<TActivity*, unsigned int, TActivity>(TActivity*, unsigned int, const TActivity&, const _STL::__false_type&);
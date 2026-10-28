// std::swap(pair<CCharString,C3DVector>& a, pair<CCharString,C3DVector>& b) -- STLport swap of two (name, point) pairs through a temporary copy
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E0C0, 121 bytes, /Gr `__fastcall` (a in ecx -> edi, b in edx -> ebx),
//   plain `ret`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CCharString::operator=(const CCharString&)
//   @ 0x0099EFB0 (twice), CCharString::~CCharString @ 0x0099EAE0 (the temporary).
// Callers: the iter_swap of a pair<CCharString,C3DVector> partition 0x0077E8F0 and a tail jump from 0x0077E3B0.
// Element: std::pair<CCharString, C3DVector> (16 bytes; Ego_r.pdb lists `_Pair_base<CCharString,C3DVector>`).
// Label correction: catalogued as `swap<C3DSkeletalBoneMorph>` (C3DSkeletalBoneMorph is 20 bytes); the element is
//   pair<CCharString, C3DVector>.
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- the temporary is built by STLport's explicit pair copy
//   constructor (string copy, then C3DVector's user memberwise copy: three direct loads), the two assignments are the
//   pair's implicit operator=
//   (string operator=, then the trivially assigned C3DVector as a three-word block through a pointer).
class C3DVector {                                       // user memberwise copy ctor, implicit (block) assignment
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<CCharString, C3DVector> >(pair<CCharString, C3DVector>&, pair<CCharString, C3DVector>&);

}
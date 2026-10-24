// _STL::swap(pair<CCharString,C3DHelperPoint>& a, pair<CCharString,C3DHelperPoint>& b) -- STLport swap of two helper-point map entries through a temporary copy
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6E5E0, 141 bytes (+3 int3), /Gr `__fastcall` (a in ecx -> edi, b in
//   edx -> ebx), plain `ret`.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::operator=(const CCharString&)
//   0x0099EFB0 (twice), CCharString::~CCharString 0x0099EAE0 (the temporary).
// Callers: a tail jump from 0x00A6EF70 and the iter_swap of the CCompareString partition 0x00A6F610 (same chunk;
//   its comparator is CCharString::FastCompare 0x009E6530, the CCompareString of CPhysicsMesh::HelperPoints).
// Label correction: catalogued as `swap<C3DSkeletalBoneMorph>` (propagated); the 20-byte element sorted by the
//   CPhysicsMesh TU's CCompareString is the HelperPoints entry pair<CCharString, C3DHelperPoint> (C3DHelperPoint =
//   C3DVector Position + long BoneIndex, Ego_r.pdb).
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` (as the landed pair<CCharString,C3DVector> swap
//   0x0077E0C0) -- the temporary is built by STLport's explicit pair copy constructor (string copy, then the implicit
//   C3DHelperPoint copy: C3DVector's user memberwise copy, three direct loads, then BoneIndex through the member
//   pointer kept for the assignment), the two assignments are the pair's implicit operator= (string operator=, then the
//   trivially assigned C3DHelperPoint as a four-word block through a pointer).
class C3DVector {                                       // user memberwise copy ctor, implicit (block) assignment
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace _STL {

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<CCharString, C3DHelperPoint> >(pair<CCharString, C3DHelperPoint>&, pair<CCharString, C3DHelperPoint>&);

}
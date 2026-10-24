// _global::_Adjust_heap(std::pair<unsigned long,C3DHelperPoint>*, long, long, std::pair<unsigned long,C3DHelperPoint>, CKeyPairCompareLess) -- Dinkumware heap sift-down for 20-byte (CRC key, helper point) pairs ordered by the unsigned key, then _Push_heap @ 0x00A31380
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a32440, 187 bytes (+ int3 padding).
// Callee: 0x00A31380 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 20-byte pair by value (built in its argument slot: `sub esp,14h ;
// mov eax,esp` then five dword stores) and the empty comparator follow (`ret 1Ch`).
// Element: Ego_r.pdb C3DHelperPoint (0x10 bytes) = C3DVector Position +0, long BoneIndex +0x0C,
// so pair<unsigned long, C3DHelperPoint> is 20 bytes (`lea esi,[eax+eax*4]` stride), as labelled.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; implicit element assignment = five dword moves; the
// key test is an UNSIGNED `jae`.  The argument copy stores the five dwords straight off eax, so
// the pair, C3DHelperPoint and C3DVector all copy member by member (user copy constructors).
namespace FableHeap_00a32440 {

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct C3DHelperPoint {
    C3DVector Position;
    long BoneIndex;
};

template<class K, class V> struct pair {
    K first;
    V second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};

typedef pair<unsigned long, C3DHelperPoint> SKeyedHelper;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedHelper& a, const SKeyedHelper& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(SKeyedHelper* first, long hole, long top, SKeyedHelper value, CKeyPairCompareLess pred);   // 0x00A31380

void __fastcall _Adjust_heap(SKeyedHelper* first, long hole, long bottom, SKeyedHelper value, CKeyPairCompareLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}
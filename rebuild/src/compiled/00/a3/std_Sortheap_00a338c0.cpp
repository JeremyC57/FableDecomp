// std::_Sort_heap(SKeyedHelper* first, SKeyedHelper* last, CKeyPairCompareLess pred) -- Dinkumware _Sort_heap over 20-byte (CRC key, C3DHelperPoint) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A338C0, length 217 bytes (+ int3 padding), `ret 4`.
// Identity: catalog `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,CKeyPairCompareLess<...>>`, consistent with the
// bytes: stride 0x14 (0x66666667 / sar 3), and the only callee is the landed `_Adjust_heap` @ 0x00A32440 of
// pair<unsigned long, C3DHelperPoint> (Ego_r.pdb C3DHelperPoint = C3DVector Position + long BoneIndex). Element
// model copied from that source.
// Callees: _Adjust_heap @ 0x00A32440 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 1Ch`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly. The
// `_Ty(*(_Last - 1))` temporary is enregistered (key in ecx, X and Y on the x87 stack, Z spilled to the frame,
// BoneIndex in edx) because the pair and C3DVector copy member by member (user copy constructors);
// `*_Dest = *_First` is the implicit assignment (five dword moves through ebp), and the value argument is
// stored field by field from the temporary (`fstp` for X/Y). _Pop_heap and _Pop_heap_0 are __forceinline: with
// plain `inline` VC7.1 keeps _Pop_heap out of line here (a local cdecl call, 133 v 217 bytes) -- its inliner
// declines it (the 0x0076AF10 sibling, whose pair has no implicit-copy member, inlines with plain `inline`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class C3DHelperPoint { public:
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

void __fastcall _Adjust_heap(SKeyedHelper* first, long hole, long bottom, SKeyedHelper value, CKeyPairCompareLess pred);   // 0x00A32440

__forceinline void _Pop_heap(SKeyedHelper* first, SKeyedHelper* last, SKeyedHelper* dest, SKeyedHelper value,
                      CKeyPairCompareLess pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

__forceinline void _Pop_heap_0(SKeyedHelper* first, SKeyedHelper* last, CKeyPairCompareLess pred, SKeyedHelper*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedHelper(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SKeyedHelper* first, SKeyedHelper* last, CKeyPairCompareLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedHelper*)0);
}
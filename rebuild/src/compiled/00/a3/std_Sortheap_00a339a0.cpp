// std::_Sort_heap(SDummyObjectPair* first, SDummyObjectPair* last, CKeyPairCompareLess pred) -- Dinkumware _Sort_heap over a (unsigned long, C3DDummyObject) vector: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A339A0, length 205 bytes (+ int3 padding), `ret 4`.
// Identity: catalog `_Sort_heap<pair<unsigned long,C3DDummyObject>*,...>`. Consistent with the bytes: stride 0x38
// (the 0x92492493 / add / sar 5 divide by 56) = a dword key + Ego_r.pdb C3DDummyObject (52 bytes: CMatrix3x4
// Transform, long BoneIndex), and the callee 0x00A32500 (manifest `_Adjust_heap<CDebugBox*,int,CDebugBox>`,
// propagated) sifts with an UNSIGNED compare (`jae`) of the key.
// Callees: _Adjust_heap @ 0x00A32500 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 40h`).
// Shape: /Gr Dinkumware TU, the 0x007590C0 shape with a 13-dword payload: `_Ty(*(_Last - 1))` is a frame
// temporary built by the pair's memberwise user copy constructor (key, then `rep movsd` x13 for the trivially
// copyable C3DDummyObject), `*_Dest = *_First` the trivial assignment (`rep movsd` x14), the value argument
// copied from the temporary the same split way; the loop calls _Pop_heap_0 directly.
class CMatrix3x4 { public:                         // 48 bytes, trivially copyable
    float m_rows[3][4];
};
class C3DDummyObject { public:                     // 52 bytes (Ego_r.pdb)
    CMatrix3x4 Transform;                   // +0x00
    long       BoneIndex;                   // +0x30
};
struct SDummyObjectPair {                   // std::pair<unsigned long, C3DDummyObject> (0x38 bytes)
    unsigned long  first;                   // +0x00
    C3DDummyObject second;                  // +0x04
    SDummyObjectPair(const SDummyObjectPair& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SDummyObjectPair& a, const SDummyObjectPair& b) const { return a.first < b.first; }
};

void __fastcall _Adjust_heap(SDummyObjectPair* first, int hole, int bottom, SDummyObjectPair value,
                             CKeyPairCompareLess pred);   // 0x00A32500

inline void _Pop_heap(SDummyObjectPair* first, SDummyObjectPair* last, SDummyObjectPair* dest, SDummyObjectPair value,
                      CKeyPairCompareLess pred, int*)
{
    *dest = *first;
    _Adjust_heap(first, 0, int(last - first), value, pred);
}

inline void _Pop_heap_0(SDummyObjectPair* first, SDummyObjectPair* last, CKeyPairCompareLess pred, SDummyObjectPair*)
{
    _Pop_heap(first, last - 1, last - 1, SDummyObjectPair(*(last - 1)), pred, (int*)0);
}

void __fastcall _Sort_heap(SDummyObjectPair* first, SDummyObjectPair* last, CKeyPairCompareLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SDummyObjectPair*)0);
}
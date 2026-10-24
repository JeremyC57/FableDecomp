// _global::__pop_heap(SKeyedHelperPoint* first, SKeyedHelperPoint* last, SKeyedHelperPoint* result, SKeyedHelperPoint value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (unsigned long, C3DHelperPoint) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32B50, length 112 bytes, `ret 20h` (result, the 20-byte value, the comparator
//   and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the row's `_Pop_heap<pair<unsigned long,CSoundPair>*,...>` is propagated; the callee 0x00A32440 carries
//   `_Adjust_heap<pair<unsigned long,C3DHelperPoint>*,int,...,CKeyPairCompareLess<unsigned long,C3DHelperPoint>>` and the
//   element is 20 bytes (`imul 66666667h ; sar edx,3`): pair<unsigned long, C3DHelperPoint> (Ego_r.pdb C3DHelperPoint:
//   C3DVector Position +0, long BoneIndex +0xc). Callers: 0x00A32D8E, 0x00A3334E, 0x00A3433D.
// Callee: __adjust_heap @ 0x00A32440.
// Shape: `*result = *first` is the pair's implicit (trivial) assignment, five dwords through edi (C3DVector has no user
//   operator=); the by-value copy is built in the argument area word by word through esi by STLport's explicit pair copy
//   constructor and the helper point's implicit memberwise copy (C3DVector has a memberwise user copy constructor).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct C3DHelperPoint {                                // 0x10 bytes
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

struct SKeyedHelperPoint {                             // std::pair<unsigned long, C3DHelperPoint>, 20 bytes
    unsigned long  first;                              // +0x00
    C3DHelperPoint second;                             // +0x04
    SKeyedHelperPoint(const SKeyedHelperPoint& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(SKeyedHelperPoint* first, long holeIndex, long len, SKeyedHelperPoint value,
                              CKeyPairCompareLess comp);   // 0x00A32440

void __fastcall __pop_heap(SKeyedHelperPoint* first, SKeyedHelperPoint* last, SKeyedHelperPoint* result,
                           SKeyedHelperPoint value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}
// std::__make_heap(pair<unsigned long, C3DHelperPoint>* first, pair<unsigned long, C3DHelperPoint>* last, CKeyPairCompareLess<unsigned long, C3DHelperPoint> comp, pair<unsigned long, C3DHelperPoint>*, int*) -- SGI make_heap of the keyed helper points: sift every parent down from (len-2)/2 to the root with __adjust_heap, each fed a by-value copy of the parent
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32C20, 113 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
// empty comparator's slot, the value-type pointer and the Distance* on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: std::__adjust_heap(first, holeIndex, len, T value, comp) @ 0x00A32440 (manifest `_Adjust_heap<pair<unsigned
// long,C3DHelperPoint>*, ...>`; /Gr: first in ecx, holeIndex in edx, then len, the 20-byte value and the comparator slot).
// Types (Ego_r.pdb): C3DHelperPoint (16 bytes) = C3DVector Position +0, long BoneIndex +0xc; the pair is 20 bytes with the
// key at +0. The comparator is empty.
// Shape: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) { __adjust_heap(first,
// parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count divides by 20 (reciprocal
// 0x66666667, `sar edx,3`); the pair's implicit copy is memberwise (C3DVector has a user copy constructor), built straight
// into the argument area (`sub esp,14h ; mov eax,esp`) from a strength-reduced `first + parent*20 + 0Ch` pointer.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint { public:                            // 16 bytes
    C3DVector Position;                            // +0x00
    long      BoneIndex;                           // +0x0c
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};

template <class K, class V>
class CKeyPairCompareLess { public:                       // empty functor, passed in a 4-byte slot
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};

typedef pair<unsigned long, C3DHelperPoint> CHelperPointPair;
typedef CKeyPairCompareLess<unsigned long, C3DHelperPoint> CHelperPointLess;

void __fastcall __adjust_heap(CHelperPointPair* first, int holeIndex, int len, CHelperPointPair value, CHelperPointLess comp);   // 0x00A32440

void __fastcall __make_heap(CHelperPointPair* first, CHelperPointPair* last, CHelperPointLess comp, CHelperPointPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, CHelperPointPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
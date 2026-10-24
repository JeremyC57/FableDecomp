// _STL::__unguarded_partition(pair<unsigned long, C3DHelperPoint>* first, pair<unsigned long, C3DHelperPoint>* last, pair<unsigned long, C3DHelperPoint> pivot, CKeyPairCompareLess comp) -- quicksort partition step of the helper-point table around the pivot's key: advance from both ends, swap out-of-place pairs, return the split point
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A329B0, 163 bytes, `ret 18h` (__fastcall: first in ecx, last in edx, the
//   20-byte pivot and the empty comparator on the stack). No callees.
// Identity: the __introsort_loop partition of the C3DMeshStats helper-point sort (20-byte family of 0x00A31280 /
//   0x00A32440 / 0x00A32C20); the catalog's "partition_swap_5uint" is descriptive.
// Element: pair<unsigned long, C3DHelperPoint> (Ego_r.pdb C3DHelperPoint = C3DVector Position + long BoneIndex; C3DVector
//   has a user copy constructor, the landed 0x00A32440 / 0x00A32C20 model), so every copy is memberwise dword moves.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` -- the pivot key kept in ecx (unsigned
//   `jae`/`jb`), __iter_swap's `T tmp = *a` with its key in esi and its second homed in the pivot's own (dead) second
//   slots, the two assignments implicit five-dword copies.
namespace _STL {

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct C3DHelperPoint {                                // 16 bytes
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DHelperPoint> TEntry;    // 20 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, C3DHelperPoint>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (TEntry*)0);
}

TEntry* __fastcall __unguarded_partition(TEntry* first, TEntry* last, TEntry pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

}
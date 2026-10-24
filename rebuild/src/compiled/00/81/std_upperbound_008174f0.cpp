// std::__upper_bound(const THitLocationIntersectionElement* first, const THitLocationIntersectionElement* last, const long& val, THitLocationIntersectionElementPred comp, int*) -- STLport binary search for the first hit-location intersection whose key is above val
// Leaf: ??$__upper_bound@
// Retail: Fable.exe @ 0x008174F0, 64 bytes, `ret 0Ch`: __fastcall (the algorithm TUs are /Gr), first in ecx -> eax,
// last in edx; &val, the empty predicate by value (4-byte slot) and the null distance-type pointer on the stack.
// Callers: the landed __equal_range 0x00817E30 (on [middle + 1, first + len), called first) and 0x0081782C. Label: catalogued
// `std_upper_bound_Binary`; the element is the anonymous-namespace THitLocationIntersectionElement (Ego_r.pdb, 32
// bytes: long key +0, CHitLocation* hit_location +4, C3DVector intersection_pos +8, outward_normal +0x14), the
// predicate compares the element's key with the long either way round (landed 0x00817E30 model).
// Shape: STLport bisection -- `len = last - first` (`sar edx,5`), `half = len >> 1`, middle = first + half,
// `comp(val, *middle)` (signed `cmp ecx,ebx ; jge`) -> len = half, else first = middle + 1, `len = len - half - 1` (the
// `or edx,-1 ; sub edx,esi` form); val is loaded once ahead of the loop.
struct CHitLocation;
struct C3DVector { float X, Y, Z; };                // 12 bytes
namespace {
struct THitLocationIntersectionElement {        // 32 bytes
    long          key;                          // +0x00
    CHitLocation* hit_location;                 // +0x04
    C3DVector     intersection_pos;             // +0x08
    C3DVector     outward_normal;               // +0x14
};
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, long b) const { return a.key < b; }
    bool operator()(long a, const THitLocationIntersectionElement& b) const { return a < b.key; }
};
}
namespace std {
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter __fastcall __upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(val, *middle))
            len = half;
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}
template const THitLocationIntersectionElement* __fastcall
__upper_bound(const THitLocationIntersectionElement*, const THitLocationIntersectionElement*, const long&,
              THitLocationIntersectionElementPred, int*);
}
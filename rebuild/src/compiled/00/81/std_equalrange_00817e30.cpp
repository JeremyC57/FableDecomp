// std::__equal_range(const THitLocationIntersectionElement* first, last, const long& val, THitLocationIntersectionElementPred comp, int*) -- the range of hit-location intersections whose key equals val
// Leaf: __equal_range
// Retail: Fable.exe @ 0x00817E30, 147 bytes, `ret 10h`: __fastcall with the hidden pair<> result pointer in ecx
// (spilled to the `push ecx` frame word), first in edx -> edi; last, &val, the empty predicate by value and the
// distance-type null on the stack. Call sites 0x00815F36 (in GetPBestHitLocationFromList 0x00815F00, named by the landed
// CTCHitLocations 0x008168B0 / 0x008169B0) and 0x008180D6, all in the anonymous-namespace TU 0x5EBD2A70; they
// pass `(last - 1)->key` as val and a zeroed THitLocationIntersectionElementPred().
// Layout (Ego_r.pdb): THitLocationIntersectionElement 32 bytes: key +0, hit_location +4, intersection_pos C3DVector +8,
//   outward_normal C3DVector +0x14. The predicate compares the element's key with the long either way round.
// Callees: __upper_bound @ 0x008174F0 (called FIRST, on [middle + 1, first + len)) and __lower_bound @ 0x008174B0 (on
//   [first, middle)), both __fastcall `ret 0Ch` with (&val, comp, (int*)0) on the stack.
// Shape: STLport bisection (`len >> 1` is a bare `sar 1`, `len - half - 1`); the straddle case builds the result as
// pair(__lower_bound(...), __upper_bound(...)) so VC's right-to-left argument order calls upper_bound first and never
// advances `first` in place (the faithful statement-by-statement form calls lower_bound first: DIFFER 156v147).
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
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter __fastcall __lower_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*);
template <class ForwardIter, class Tp, class Compare, class Distance>
ForwardIter __fastcall __upper_bound(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*);
template <class ForwardIter, class Tp, class Compare, class Distance>
pair<ForwardIter, ForwardIter> __fastcall
__equal_range(ForwardIter first, ForwardIter last, const Tp& val, Compare comp, Distance*)
{
    Distance len = last - first;
    Distance half;
    ForwardIter middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else if (comp(val, *middle))
            len = half;
        else {
            return pair<ForwardIter, ForwardIter>(__lower_bound(first, middle, val, comp, (Distance*)0),
                                                  __upper_bound(middle + 1, first + len, val, comp, (Distance*)0));
        }
    }
    return pair<ForwardIter, ForwardIter>(first, first);
}
template pair<const THitLocationIntersectionElement*, const THitLocationIntersectionElement*> __fastcall
__equal_range(const THitLocationIntersectionElement*, const THitLocationIntersectionElement*, const long&,
              THitLocationIntersectionElementPred, int*);
}
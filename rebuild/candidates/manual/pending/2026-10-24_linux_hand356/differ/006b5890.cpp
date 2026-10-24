// _STL::__lower_bound(pair<long, float>* first, pair<long, float>* last, const pair<long, float>& val, less<pair<long, float> > comp, int*) -- SGI binary search for the first (long, float) pair not less than `val` under the lexicographic pair operator< (key signed, then the float)
// Leaf: __lower_bound
// Grade: DIFFER(76v76) -- x87 allocation only: retail hoists `fld [val+4]` above the loop and compares `fcom [middle+4]` (test ah,41h), popping st(0) at the exit; ours keeps &val in ebx and reloads val.second on the equal-key path (`fld [middle+4] ; fcomp [ebx+4]`, test ah,5 / jp). Tried: comparator / operator< by value (worse: spills), `y.second > x.second` (fld [ebx+4] ; fcomp [middle+4], still not hoisted), QFE 4035 (same). Next: a caller-side copy or a local float of val.second in a sibling STLport build; check other float-keyed __lower_bound bodies in retail.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5890, 76 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, the
// value reference, the empty comparator's slot and the Distance* on the stack -- the SGI/STLport algorithm TUs are /Gr).
// No callees; the single caller is 0x006B5A62 (a lower_bound front end).
// Shape: STLport `len = last - first; while (len > 0) { half = len >> 1; middle = first + half; if (comp(*middle, val))
// { first = middle + 1; len = len - half - 1; } else len = half; } return first;` with less<pair> inlined to the pair
// operator< `a.first < b.first || (!(b.first < a.first) && a.second < b.second)`: val.first stays in edi and val.second
// on the x87 stack for the whole loop (`fcom` against middle.second, popped once at the end).
// Label: catalogued as Dinkumware `_Lower_bound<pair<long,float>*, pair<long,float>, int>`.
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T>
struct less {                                      // empty functor, passed in a 4-byte slot
    bool operator()(const T& x, const T& y) const { return x < y; }
};

typedef pair<long, float> CKeyFloatPair;

CKeyFloatPair* __fastcall __lower_bound(CKeyFloatPair* first, CKeyFloatPair* last, const CKeyFloatPair& val,
                                        less<CKeyFloatPair> comp, int*)
{
    int len = last - first;
    int half;
    CKeyFloatPair* middle;

    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

// std::__linear_insert(SAttitudeEntry* first, SAttitudeEntry* last, SAttitudeEntry val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the CSubtitledMessage attitude map's 8-byte pair<float, EConversationAttitude>: a value whose key is below the first element shifts [first, last) up one slot and goes first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0F10, 105 bytes (+ int3 padding), `ret 0Ch`.
// Callee: __unguarded_linear_insert(last, val, comp) 0x004CFCB0 (`__fastcall`, last in ecx, the 8-byte value and the
//   comparator on the stack).
// Label correction: the catalog's `InsertionSort_MoveElementBackward` is descriptive; the body is STLport's
//   `__linear_insert` for the CVectorMap<float, EConversationAttitude, CKeyPairCompareLess<...>> element (Ego_r.pdb).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the value and the empty comparator on
//   the stack (8 + 4 = 0x0C).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` with copy_backward inlined to STLport's random-access
//   `__copy_backward` (`for (n = last - first; n > 0; --n) *--result = *--last;`, the pair's implicit operator= as
//   two dword moves); the by-value argument of the else arm is built in the argument area through pair's memberwise
//   copy constructor. `fld val.first ; fcomp [first] ; test ah,5 ; jp` = val.first < first->first.
namespace std {

enum EConversationAttitude { ATTITUDE_NONE };

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<float, EConversationAttitude> SAttitudeEntry;   // 8 bytes

struct CKeyPairCompareLess {                    // CKeyPairCompareLess<float, EConversationAttitude>
    bool operator()(const SAttitudeEntry& a, const SAttitudeEntry& b) const { return a.first < b.first; }
};

template <class RandomAccessIter, class BidirectionalIter, class Distance>
inline BidirectionalIter __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                         const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

inline SAttitudeEntry* copy_backward(SAttitudeEntry* first, SAttitudeEntry* last, SAttitudeEntry* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __unguarded_linear_insert(SAttitudeEntry* last, SAttitudeEntry val, CKeyPairCompareLess comp);   // 0x004CFCB0

void __fastcall __linear_insert(SAttitudeEntry* first, SAttitudeEntry* last, SAttitudeEntry val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}
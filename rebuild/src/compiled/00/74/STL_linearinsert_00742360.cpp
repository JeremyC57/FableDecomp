// _STL::__linear_insert(pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* first, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* last, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> val, CKeyPairCompareLess comp) -- insert one mode-registry entry: a key below the first shifts the run up one slot, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00742360, 99 bytes (+13 int3), `ret 0Ch` (link-time __fastcall: first in ecx, last in
//   edx, the 8-byte value and the empty comparator on the stack).
// Callee: __unguarded_linear_insert(last, val, comp) @ 0x0073CFC0 (__fastcall, out of line, `ret 0Ch`).
// Caller: the landed __insertion_sort 0x00743430 (it calls this address for every element after the first).
// Label: the manifest's "InsertionSort_Pair_Step" is descriptive; the body is STLport `__linear_insert` (value by value).
// Element: the 8-byte pair of the CTCCreatureModeManager mode registry (CVectorMap<unsigned long, CRegisteredModeInfo>).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the pair's assignment is not trivial to STLport's type traits (user
//   copy constructor), so copy_backward is the random-access `__copy_backward` count loop (`n = last - first` by
//   `sar 3`, `*--result = *--last`); the else arm rebuilds the by-value pair in its outgoing argument slot.
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    struct CRegisteredModeInfo {                         // Ego_r.pdb 4 bytes
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};

namespace _STL {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {  // empty, passed by value
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModePair;
typedef CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModeLess;

namespace _STL {
struct random_access_iterator_tag {};

template <class RandomAccessIter, class BidirectionalIter, class Distance>
inline BidirectionalIter __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                         const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

inline TModePair* copy_backward(TModePair* first, TModePair* last, TModePair* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __unguarded_linear_insert(TModePair* last, TModePair val, TModeLess comp);   // 0x0073CFC0

void __fastcall __linear_insert(TModePair* first, TModePair* last, TModePair val, TModeLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}
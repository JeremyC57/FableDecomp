// std::__introsort_loop(pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* first, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* last, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>*, int depth_limit, CKeyPairCompareLess comp) -- quicksort the mode-registry entries around a median-of-three pivot until the runs are 16 or shorter, falling back to a heap sort at the depth limit
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743EC0, 196 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, last in edx,
//   then the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Callees: __unguarded_partition(first, last, pivot, comp) @ 0x0073DE50 (__fastcall, `ret 0Ch`), itself (the right
//   half), __partial_sort @ 0x00743470 (the depth-limit fallback).
// Caller: the landed sort 0x007441D0.
// Label: the manifest's "Sort_IntrosortPhase" is descriptive; the body is STLport `__introsort_loop`.
// Element: the 8-byte pair of the CTCCreatureModeManager mode registry (CVectorMap<unsigned long, CRegisteredModeInfo>).
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last - first)/2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (T*)0, depth_limit, comp); last = cut; }` -- the length test
//   is the byte form (`and -8 ; cmp 80h`), the depth limit is decremented in its argument slot, __median returns a
//   reference (the three keys in ecx/edx/ebp, the chosen address in eax) and the pivot copy is built in the argument slot.
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    class CRegisteredModeInfo { public:                         // Ego_r.pdb 4 bytes
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};

namespace std {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {  // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModePair;
typedef CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModeLess;

namespace std {
template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

TModePair* __fastcall __unguarded_partition(TModePair* first, TModePair* last, TModePair pivot, TModeLess comp);       // 0x0073DE50
void __fastcall __partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModePair*, TModeLess comp);      // 0x00743470

inline void partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModeLess comp)
{
    __partial_sort(first, middle, last, (TModePair*)0, comp);
}

void __fastcall __introsort_loop(TModePair* first, TModePair* last, TModePair*, int depth_limit, TModeLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        TModePair* cut = __unguarded_partition(first, last,
                                               TModePair(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                               comp);
        __introsort_loop(cut, last, (TModePair*)0, depth_limit, comp);
        last = cut;
    }
}
}
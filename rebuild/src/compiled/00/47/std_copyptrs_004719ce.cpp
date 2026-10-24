// std::__copy_ptrs(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>* result, const __false_type&) -- STLport: assign [first, last) over result element by element (the non-memmove copy), return the end of the destination
// Leaf: __copy_ptrs
// Retail: Fable.exe (TLC, VC7.1) @ 0x004719CE, 59 bytes, `ret 8` (__fastcall: first in ecx, last in edx; result and the
//   __false_type tag on the stack).
// Identity: the catalog's "VectorInsert_CopyAnimations" is behavioural. Both callers are the vector operator= @ 0x00471865
//   of CAnimationSet::Anims (CVectorMap<unsigned long, CAnimationEntry>, element pair<unsigned long, CAnimationEntry>,
//   20 bytes); the 20-byte allocate-and-copy beside it is 0x0047197A.
// Callees: CAnimationEntry::operator=(const CAnimationEntry&) @ 0x00471A09 (landed; out of line).
// Model: STLport 4.x `__copy_ptrs(.., const __false_type&)` forwarding to the inlined random-access
//   `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)`: `for (n = last - first; n > 0; --n)
//   *result++ = *first++` (the `idiv` by 0x14). The element assignment is pair's implicit member-wise operator= (the key
//   dword, then CAnimationEntry::operator= on +4), spelled out `__forceinline` because a size TU leaves it out of line.
// Shape: `push 14h ; pop edi` = size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)

class CAnimComponentMap;
class CAnimationEntry {
public:
    unsigned long      BankIndex;                   // +0x00
    long               AnimName;                    // +0x04 CDefString
    long               GroupName;                   // +0x08 CDefString
    CAnimComponentMap* Components;                  // +0x0C CIVCountedPointer<CAnimComponentMap>
    CAnimationEntry& operator=(const CAnimationEntry& other);   // 0x00471A09
};

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };

template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    __forceinline pair& operator=(const pair<T1, T2>& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }
};

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

typedef pair<unsigned long, CAnimationEntry> _AnimEntry;
template _AnimEntry* __fastcall __copy_ptrs(_AnimEntry* first, _AnimEntry* last, _AnimEntry* result, const __false_type&);
}
// std::__copy(CAnimationEntryPair* first, CAnimationEntryPair* last, CAnimationEntryPair* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of (CRC, CAnimationEntry) pairs: assign [first, last) onto result and return the end of the output
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E18B0, length 148 bytes (+ int3 padding), `ret 0Ch`.
// Identity / label correction: the catalog label `_Sort_heap<pair<unsigned long,CAnimationEntry>*,...>` is
// propagated; the bytes are not a heap sort (no pop/adjust callee, no comparator). They are STLport's
// `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)`: __fastcall first/last in ecx/edx,
// result, the tag reference and the Distance* on the stack (`ret 0Ch`), the count `(last - first) / 0x14`
// (0x66666667 / sar 3) driving `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`
// (the count lives in the dead tag slot) and `result` returned in eax (also on the n <= 0 path).
// Callees: the component map's virtual destroy (vtable slot 1) when its reference count (+4) drops to zero.
// Shape: the pair's implicit copy assignment inline -- the key, BankIndex, the two one-word CDefStrings, then the
// intrusive pointer's operator= (self test, release, null store, store, count bump); VC7.1 keeps second induction
// registers at the Components members (`lea esi,[ebp+10h]` / `lea edi,[ebx+10h]`). Element model from the landed
// std::fill @ 0x005E1820 (Ego_r.pdb CAnimationEntry).
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

class CDefString { public:                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
class CAnimComponentMap : public CIVCountedPointeeBase { public: };
class CAnimationEntry { public:                        // 16 bytes
    long                                 BankIndex;     // +0x00
    CDefString                           AnimName;      // +0x04
    CDefString                           GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0C
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
struct random_access_iterator_tag { };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
typedef pair<unsigned long, CAnimationEntry> CAnimationEntryPair;
template CAnimationEntryPair* __fastcall __copy(CAnimationEntryPair*, CAnimationEntryPair*, CAnimationEntryPair*,
                                                const random_access_iterator_tag&, int*);
}
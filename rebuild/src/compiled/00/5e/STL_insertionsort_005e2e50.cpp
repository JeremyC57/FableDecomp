// std::__insertion_sort(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport insertion sort of the CRC-keyed animation entries: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: ?__insertion_sort@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2E50, 92 bytes (+ padding to 96), `ret 4` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebp, last in edx -> ebx, the empty comparator on the stack, re-read from its slot every pass).
// Callees: 0x005E2770 __linear_insert (same instantiation).
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`
//   -- the by-value `*i` is built in the argument area by the pair's explicit copy ctor (key, then the entry's implicit
//   memberwise copy: three dwords and the counted pointer's add-ref copy), the source fields read through a second
//   induction pointer at +0x0c (`lea esi,[edi+0Ch]`); same template as the landed __insertion_sort 0x005E2E00.
// Label correction: catalogued as `std_HeapInitCopyPairs`; the body is STLport `__insertion_sort`.
class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CAnimationEntry> SAnimationEntryPair;   // 0x14 bytes: CRC key + entry

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

void __fastcall __linear_insert(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair val, TAnimationEntryLess comp);   // 0x005E2770

void __fastcall __insertion_sort(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp)
{
    if (first == last)
        return;
    for (SAnimationEntryPair* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
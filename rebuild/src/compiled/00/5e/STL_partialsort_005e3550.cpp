// _STL::__partial_sort(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* middle, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>*, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport partial_sort core for the CRC-keyed animation entries: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3550, 117 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebx, middle in edx -> edi, then last, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x005E2320 __make_heap (first, middle, comp, (T*)0, (int*)0), 0x005E21F0 __pop_heap (landed), 0x005E2FB0
//   sort_heap (landed).
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first,
//   middle, comp);` -- the unsigned key test (`jae`) reuses the loaded key for the 20-byte by-value copy built in the
//   argument area; `last` is re-read from its slot each pass.
// Label correction: catalogued as `std_sort_IntrosortPartition20Byte_Callback`; the body is STLport `__partial_sort`.
struct CAnimComponentMap {                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
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

struct CDefString {                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

struct CAnimationEntry {                                   // Ego_r.pdb, 0x10 bytes
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

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

void __fastcall __make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp, SAnimationEntryPair*, int*);   // 0x005E2320
void __fastcall __pop_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                           SAnimationEntryPair val, TAnimationEntryLess comp, int*);   // 0x005E21F0
void __fastcall sort_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp);   // 0x005E2FB0

inline void make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp)
{
    __make_heap(first, last, comp, (SAnimationEntryPair*)0, (int*)0);
}

void __fastcall __partial_sort(SAnimationEntryPair* first, SAnimationEntryPair* middle, SAnimationEntryPair* last, SAnimationEntryPair*, TAnimationEntryLess comp)
{
    make_heap(first, middle, comp);
    for (SAnimationEntryPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SAnimationEntryPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
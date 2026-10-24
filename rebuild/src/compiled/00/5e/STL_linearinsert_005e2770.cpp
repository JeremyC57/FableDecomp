// _STL::__linear_insert(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry> val, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport insertion-sort step for the CRC-keyed animation entries: a new minimum shifts the whole prefix up one slot, anything else goes through the unguarded insert
// Leaf: ?__linear_insert@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2770, 121 bytes (+ padding to 128), `ret 18h` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, last in edx, the 20-byte val by value and the empty comparator on the stack).
// Callees: 0x005E0E70 __copy_backward (landed; tagged form), 0x005E0E20 pair<unsigned long, CAnimationEntry>::operator=
//   (landed), 0x005E10B0 __unguarded_linear_insert (same instantiation), the component map's virtual slot 1.
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU
//   (called by __insertion_sort 0x005E2E50).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward is inlined down to the tagged
//   `__copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0)` call, its empty tag temporary homed in
//   the dead comparator slot (`lea eax,[esp+20h]`); the by-value val's counted pointer is released once at the join.
// Label correction: catalogued as `std_MoveElement_Pair`; the body is STLport `__linear_insert`.
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

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SAnimationEntryPair* __fastcall __copy_backward(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result,
                                                const random_access_iterator_tag&, int*);   // 0x005E0E70
void __fastcall __unguarded_linear_insert(SAnimationEntryPair* last, SAnimationEntryPair val, TAnimationEntryLess comp);   // 0x005E10B0

inline SAnimationEntryPair* copy_backward(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(SAnimationEntryPair* first, SAnimationEntryPair* last, SAnimationEntryPair val, TAnimationEntryLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
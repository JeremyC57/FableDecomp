// std::swap(pair<unsigned long, CAnimationEntry>& a, pair<unsigned long, CAnimationEntry>& b) -- STLport swap of two CRC-keyed animation entries through a counted temporary
// Leaf: ?swap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1020, 132 bytes (+ padding to 144), plain `ret` (__fastcall /Gr algorithm TU: a in ecx,
//   b in edx -> esi).
// Callees: 0x005E0E20 pair<unsigned long, CAnimationEntry>::operator= (landed) for `a = b`; the component map's virtual
//   slot 1 (the temporary's final release tail-jumps into it).
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU (called
//   by __unguarded_partition 0x005E2100 as iter_swap).
// Shape: STLport `_Tp tmp = a; a = b; b = tmp;` -- tmp lives in ebx/ebp/edi plus two spilled frame words (the explicit
//   pair copy ctor add-refs the pointee), VC7.1 calls the implicit pair operator= out of line for `a = b` and expands it
//   inline for `b = tmp` (four dword stores, then the counted pointer's self test / release + zero store / adopt), and
//   tmp's destructor releases without a zero store.
// Label correction: catalogued as `swap<CAnimationEntry>`; the element is pair<unsigned long, CAnimationEntry>.
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

void __fastcall swap(SAnimationEntryPair& a, SAnimationEntryPair& b)
{
    SAnimationEntryPair tmp = a;
    a = b;
    b = tmp;
}
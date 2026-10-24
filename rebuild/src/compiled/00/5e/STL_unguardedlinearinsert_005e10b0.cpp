// _STL::__unguarded_linear_insert(pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry> val, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport insertion step for the CRC-keyed animation entries: shift greater entries up one slot until val's place, then store it
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E10B0, 186 bytes (+ padding to 192), `ret 18h` (__fastcall /Gr algorithm TU: last in ecx ->
//   esi, the 20-byte val by value and the empty comparator on the stack). No direct callees (the component map's
//   virtual slot 1 on the last reference).
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU
//   (called by __linear_insert 0x005E2770).
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- the val key stays in ebp (unsigned `jae`/`jb`), both assignments are the pair's implicit operator= INLINED
//   (four dwords, then the counted pointer's self test / release + zero store / adopt + count bump; the moving copy
//   reads through a second induction pointer at next+0x10), and val's own pointer is released at the end.
// Label correction: catalogued as `_Move<CPhysicsMeshInfo*,CPhysicsMeshInfo*>` (propagated).
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

void __fastcall __unguarded_linear_insert(SAnimationEntryPair* last, SAnimationEntryPair val, TAnimationEntryLess comp)
{
    SAnimationEntryPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
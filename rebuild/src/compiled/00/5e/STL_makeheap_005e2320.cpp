// _STL::__make_heap(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp, pair<unsigned long, CAnimationEntry>*, int*) -- STLport make_heap core for the CRC-keyed animation entries: sift every parent down, last parent first
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E2320, 120 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   ebp, last in edx, then the empty comparator and the two null type-tag pointers on the stack).
// Callees: 0x005E1CB0 __adjust_heap (same instantiation; manifest `_Adjust_heap<pair<unsigned long,CAnimationEntry>*,...>`).
// Instantiation: the CVectorMap<unsigned long, CAnimationEntry> (CAnimationSet::Anims) sort of the animation TU
//   (called by __partial_sort 0x005E3550).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the count
//   is the /20 reciprocal multiply, the element is addressed through a strength-reduced pointer at its +0x0c
//   (`lea esi,[ebp+eax*4+0Ch]`, stepped by -0x14) and copied into the argument area by the pair's copy ctor.
// Label correction: catalogued as `STL_BuildHeap_Pair20Byte`; the body is STLport `__make_heap`.
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

void __fastcall __adjust_heap(SAnimationEntryPair* first, int holeIndex, int len, SAnimationEntryPair val, TAnimationEntryLess comp);   // 0x005E1CB0

void __fastcall __make_heap(SAnimationEntryPair* first, SAnimationEntryPair* last, TAnimationEntryLess comp, SAnimationEntryPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SAnimationEntryPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
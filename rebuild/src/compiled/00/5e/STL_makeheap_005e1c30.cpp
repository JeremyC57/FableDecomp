// _STL::__make_heap(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int*) -- STLport make_heap core for anim-component map entries: sift every parent down, last parent first
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1C30, 81 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx, then the empty comparator and the two null type-tag pointers on the stack).
// Callees: 0x005E1210 __adjust_heap (same instantiation; manifest `_Adjust_heap<pair<EAnimComponent,...>>`).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU
//   (called by __partial_sort 0x005E2F40).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the
//   pair's explicit copy constructor builds the element in the argument area straight from `first[parent]`
//   (`[edi+esi*8]`, no strength-reduced pointer here).
// Label correction: catalogued as `STL_BuildHeap_PairRefCounted`; the body is STLport `__make_heap`.
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
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

template <class T1, class T2> struct pair {                // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > SAnimComponentPair;   // 8 bytes

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TAnimComponentLess;

void __fastcall __adjust_heap(SAnimComponentPair* first, int holeIndex, int len, SAnimComponentPair val, TAnimComponentLess comp);   // 0x005E1210

void __fastcall __make_heap(SAnimComponentPair* first, SAnimComponentPair* last, TAnimComponentLess comp, SAnimComponentPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SAnimComponentPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
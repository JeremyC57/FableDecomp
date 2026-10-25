// std::__introsort_loop(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, int depth_limit, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport introsort recursion for anim-component map entries: median-of-three partitions down to 16 entries, heap sort once the depth budget is spent
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E37D0, 203 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx -> ebx, then the null value-type pointer, the depth budget and the empty comparator on the stack).
// Callees: 0x005E19D0 __unguarded_partition, itself (recursion on the right part), 0x005E2F40 __partial_sort
//   (partial_sort(first, last, last, comp) inlined to its core).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` --
//   here __median is INLINED (the three keys in ecx/edx/ebp, the nested-if selection of first/middle/last-1 into eax),
//   and the pair's explicit copy constructor builds the pivot from the selected entry in the argument area.
// Label correction: catalogued as `std_sort_IntrosortPartition8Byte`; the body is STLport `__introsort_loop`.
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

SAnimComponentPair* __fastcall __unguarded_partition(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair pivot, TAnimComponentLess comp);   // 0x005E19D0
void __fastcall __partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp);   // 0x005E2F40

inline void partial_sort(SAnimComponentPair* first, SAnimComponentPair* middle, SAnimComponentPair* last, TAnimComponentLess comp)
{
    __partial_sort(first, middle, last, (SAnimComponentPair*)0, comp);
}

inline const SAnimComponentPair& __median(const SAnimComponentPair& a, const SAnimComponentPair& b, const SAnimComponentPair& c, TAnimComponentLess comp)
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

void __fastcall __introsort_loop(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair*, int depth_limit, TAnimComponentLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SAnimComponentPair* cut = __unguarded_partition(first, last,
                                                        SAnimComponentPair(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                                        comp);
        __introsort_loop(cut, last, (SAnimComponentPair*)0, depth_limit, comp);
        last = cut;
    }
}
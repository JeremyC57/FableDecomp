// std::__unguarded_insertion_sort_aux(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >*, CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > comp) -- STLport unguarded insertion sort of anim-component map entries: each element goes through __unguarded_linear_insert(i, _Tp(*i), comp)
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0FA0, 59 bytes (+ padding to 64), `ret 8` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, last in edx -> ebx, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x005E07F0 __unguarded_linear_insert (landed, same instantiation).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component TU.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` -- the pair's explicit
//   copy constructor builds the by-value element in the argument area (`sub esp,8 ; mov ecx,esp`: the key, then the
//   pointee with its count bumped behind a null test); the comparator, loaded once into edi, is pushed first.
// Label correction: catalogued as `_Copy_impl<CComplexNumber*,CComplexNumber*>` (propagated).
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

void __fastcall __unguarded_linear_insert(SAnimComponentPair* last, SAnimComponentPair val, TAnimComponentLess comp);   // 0x005E07F0

void __fastcall __unguarded_insertion_sort_aux(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair*, TAnimComponentLess comp)
{
    for (SAnimComponentPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SAnimComponentPair(*i), comp);
}
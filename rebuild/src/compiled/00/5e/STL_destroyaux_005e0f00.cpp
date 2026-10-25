// std::__destroy_aux(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, const __false_type&) -- STLport range destroy of anim-component map entries: release every counted component pointer
// Leaf: __destroy_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0F00, 52 bytes (+ padding to 64), `ret 4` (__fastcall /Gr algorithm TU: first in ecx ->
//   esi, last in edx -> edi, the `const __false_type&` tag on the stack). No direct callees (the pointee's virtual
//   slot 1 on the last reference).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > of the anim-component TU.
// Shape: STLport `for (; first != last; ++first) _Destroy(&*first);` -- the pair's implicit destructor runs the
//   counted pointer's inline release (null test, `dec [obj+4]`, slot 1 at zero) and its zero store survives because
//   the element lives on in the buffer (the same destructor's store is dead and dropped for a dying stack temporary).
// Label correction: catalogued as `_Copy_backward<CDefPointer<CThingBaseDef const>*,...>` (propagated).
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

struct __false_type { };

template <class T> inline void _Destroy(T* pointer)
{
    pointer->~T();
}

void __fastcall __destroy_aux(SAnimComponentPair* first, SAnimComponentPair* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
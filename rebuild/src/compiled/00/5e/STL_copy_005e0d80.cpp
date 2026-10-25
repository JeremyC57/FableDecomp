// std::__copy(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* first, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* last, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of anim-component map entries; returns the end of the output
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0D80, 91 bytes (+ padding to 96), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
//   edi, last in edx, then result, the tag reference and the null Distance* on the stack). No direct callees (the
//   pointee's virtual slot 1 on the last reference).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > of the anim-component TU (the
//   landed 0x005E18B0 is the same template over pair<unsigned long, CAnimationEntry>).
// Shape: STLport `for (Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`
//   -- the count `(last - first) >> 3` in ebx, the implicit pair operator= inline (key store, then the counted
//   pointer's self test / release + zero store / adopt + count bump), result returned in eax on both paths.
// Label correction: catalogued as `_Copy_backward<CDefPointer<CThingBaseDef const>*,...>` (propagated); the body is a
//   forward STLport `__copy` over the 8-byte anim-component pair.
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

struct random_access_iterator_tag { };

SAnimComponentPair* __fastcall __copy(SAnimComponentPair* first, SAnimComponentPair* last, SAnimComponentPair* result,
                                      const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
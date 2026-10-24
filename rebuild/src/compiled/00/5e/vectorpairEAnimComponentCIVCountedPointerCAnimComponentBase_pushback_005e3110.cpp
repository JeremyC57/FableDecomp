// vector<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >::push_back(const pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >& x) -- STLport vector push_back of an anim-component map entry: copy-construct at the end when there is room, else grow through _M_insert_overflow
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3110, 65 bytes, `ret 4` (__fastcall).
// Callees: 0x005E29D0 _M_insert_overflow(pos, x, __false_type(), 1, true) (manifest "std::vector::_Reallocate").
// Callers: nine sites 0x005DE55F..0x005DEABA of the CAnimationEntry::AddComponent TU (the component map's vector).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- _Construct is the placement `new (p) T(x)` with
//   its null test, the pair's explicit copy ctor copies the key and add-refs the pointee; the `__false_type` tag
//   temporary is homed in x's own argument slot (`lea edx,[esp+0Ch]`), like the landed push_back 0x005E1EB0 of this TU.
// Label: catalogued as `Vector_PushBack`.
#include <new>
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

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

template <class T> struct vector {                         // STLport vector: {start, finish, end of storage}
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x005E29D0
    void push_back(const T& x);
};

template <class T> inline void _Construct(T* p, const T& value)
{
    new (p) T(value);
}

template <class T> void vector<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        _Construct(_M_finish, x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
}

template struct vector<SAnimComponentPair>;
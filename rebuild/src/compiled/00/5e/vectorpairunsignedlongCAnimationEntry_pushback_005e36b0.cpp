// vector<pair<unsigned long, CAnimationEntry> >::push_back(const pair<unsigned long, CAnimationEntry>& x) -- STLport vector push_back of a CRC-keyed animation entry: copy-construct at the end when there is room, else grow through _M_insert_overflow
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E36B0, 83 bytes, `ret 4` (__fastcall).
// Callees: 0x005E31F0 _M_insert_overflow(pos, x, __false_type(), 1, true) (landed as CTargetPointVectorReserveAndCopy).
// Callers: 0x005DD346 (CAnimationSet) and 0x005E38FD.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement copy (null-tested) runs the pair's
//   explicit copy ctor: key, three entry dwords through esi, then the component pointer's add-ref; the `__false_type`
//   tag temporary is homed in x's own argument slot (`lea edx,[esp+0Ch]`), as the 8-byte twin 0x005E3110.
// Label: catalogued as `std_Vector_PushBack_Inline`.
#include <new>
class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
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

class CDefString { public:                                        // one word: the def string table position
    long TablePos;                                         // +0x00
};

class CAnimationEntry { public:                                   // Ego_r.pdb, 0x10 bytes
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

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> TAnimationEntryLess;

struct __false_type { __false_type() { } };                // user constructor: no zero store into the tag byte

template <class T> struct vector {                         // STLport vector: {start, finish, end of storage}
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x005E31F0
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

template struct vector<SAnimationEntryPair>;
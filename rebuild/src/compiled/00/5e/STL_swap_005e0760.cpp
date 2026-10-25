// std::swap(pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >& a, pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >& b) -- STLport swap of two anim-component map entries through a counted temporary
// Leaf: ?swap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0760, 131 bytes (+ padding to 144), plain `ret` (__fastcall /Gr algorithm TU: a in ecx ->
//   esi, b in edx -> edi). No direct callees: the pointee's virtual slot 1 (Delete on the last reference), the final
//   release of the temporary tail-jumps into it (`jmp [edx+4]`).
// Instantiation: the CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort of the anim-component
//   TU (signed enum key; called by __unguarded_partition 0x005E19D0 as iter_swap). Element model of the landed
//   __unguarded_linear_insert 0x005E07F0 / __push_heap 0x005E0940.
// Shape: STLport `_Tp tmp = a; a = b; b = tmp;` -- tmp is kept in registers (ebp = key, ebx = pointee with its count
//   bumped by the pair's explicit copy ctor), the implicit pair operator= runs the inline CIVCountedPointer::operator=
//   twice (self test, release + zero store, adopt + count bump), then tmp's destructor releases without a zero store.
// Label correction: catalogued as `swap<CAnimationEntry>` (propagated); the element is the 8-byte anim-component pair.
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

void __fastcall swap(SAnimComponentPair& a, SAnimComponentPair& b)
{
    SAnimComponentPair tmp = a;
    a = b;
    b = tmp;
}
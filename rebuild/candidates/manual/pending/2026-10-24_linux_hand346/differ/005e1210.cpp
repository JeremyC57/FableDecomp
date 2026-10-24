// _global::_Adjust_heap(pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*, long, long, pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >, CKeyPairCompareLess<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >) -- heap sift-down of an animation's component table by component type, then _Push_heap @ 0x005E0940
// Grade: DIFFER(221v221) -- esi/edi swapped throughout (retail first=esi, idx=edi and the cached value pointer in edi; ours the reverse); same instructions and length
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1210, 221 bytes.
// Callee: 0x005E0940 _Push_heap (same instantiation); CAnimComponentBase::Destroy is virtual slot 1.
// Link-time __fastcall: first in ecx, hole in edx, then bottom, the 8-byte pair by value and the
// 4-byte comparator; `ret 0x10`.  `top` lives in the `push ecx` slot.
// Element (8 bytes): EAnimComponent key +0, CIVCountedPointer<CAnimComponentBase> +4.  The pointee
// is a CIVCountedPointeeBase (vptr +0, IVRefCount +4; engine/CIVCountedPointeeBase.h is a flattened
// POD with no virtuals, so the pointee is modelled locally as a polymorphic class).  The inline
// counted-pointer assignment is the shape of the landed 0x00BBC310 model: same pointer -> nothing;
// else release the old one (`--IVRefCount == 0` -> Destroy()), null it, take the new one and add a
// reference.  The by-value pair is copy-built in the _Push_heap slot (key, pointer, add-ref) and its
// pointer is released here afterwards (the cached pointer stays in edi across the call).
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase {                      // : CIVCountedPointeeBase
public:
    virtual void v000();
    virtual void Destroy();                     // slot 1 (+0x04)
    unsigned long IVRefCount;                   // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Destroy();
    }
};

template<class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        T* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x005E0940

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

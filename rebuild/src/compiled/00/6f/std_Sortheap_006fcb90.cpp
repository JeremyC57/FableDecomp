// std::_Sort_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess pred) -- Dinkumware _Sort_heap over (unsigned long key, CSmallVector<NAnimationEvents::CEvent,32>) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FCB90, length 238 bytes (+ int3 padding), `ret 4`.
// Identity: catalog `_Sort_heap<pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32>>*,CKeyPairCompareLess<...>>`,
// consistent with the bytes: the callees are the landed `_Adjust_heap` @ 0x006FBD60 and CSmallVector<CEvent,32>::Copy
// @ 0x006FB5C0 of that instantiation (Ego_r.pdb CSmallVector<T,32>: T* Start, int Size; the pair is 12 bytes).
// Callees: CSmallVector::Copy @ 0x006FB5C0 (x3), _Adjust_heap @ 0x006FBD60 (out of line, __fastcall: first in ecx,
// hole 0 in edx, `ret 14h`), operator delete[] @ 0x00BFEB1C.
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap __forceinline (plain inline leaves _Pop_heap a local call) and the loop calling _Pop_heap_0 directly. The
// `_Ty(*(_Last - 1))` frame temporary is the pair copy: the key, then the small vector's copy constructor (zero both
// members, Copy); `*_Dest = *_First` is the implicit pair assignment (key, then CSmallVector::operator= with the
// `&other != this` self-test around Copy); the value argument is copy-constructed from the temporary the same way,
// and after _Adjust_heap the temporary's destructor runs Clear() inline (`delete[] Start`, then Start = Size = 0:
// the two zero stores after the call). Model of the landed 0x006FBD60 / 0x006FB5C0 sources.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)

namespace NAnimationEvents { class CEvent { public: long m_a, m_b; }; }

template<class T, int N> class CSmallVector { public:
    T* Start;                                   // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    ~CSmallVector() { Clear(); }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedEvents& a, const SKeyedEvents& b) const { return a.first < b.first; }
};

void __fastcall _Adjust_heap(SKeyedEvents* first, long hole, long bottom, SKeyedEvents value, CKeyPairCompareLess pred);   // 0x006FBD60

__forceinline void _Pop_heap(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* dest, SKeyedEvents value,
                      CKeyPairCompareLess pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

__forceinline void _Pop_heap_0(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess pred, SKeyedEvents*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedEvents(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedEvents*)0);
}
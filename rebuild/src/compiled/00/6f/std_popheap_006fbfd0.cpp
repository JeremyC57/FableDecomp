// std::__pop_heap(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* result, SKeyedEvents value, CKeyPairCompareLess comp, long*) -- SGI/STLport heap pop over (unsigned long key, CSmallVector<NAnimationEvents::CEvent,32>) pairs
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FBFD0, 118 bytes, `ret 0x18` (/Gr).
// Label correction: catalog "std_heapify_PopElement" -> std::__pop_heap (SGI form with the distance-type tag).
// Callees: CSmallVector::Copy 0x006FB5C0, __adjust_heap 0x006FBD60, operator delete[] 0x00BFEB1C.
// Shape: `*result = *first; __adjust_heap(first, 0, last - first, value, comp);` with the value copied into the call's
//   argument slot (copy constructor built in place).
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

template<class T, int N> struct CSmallVector {
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
    ~CSmallVector() { delete[] Start; }
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

void __fastcall __pop_heap(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* result, SKeyedEvents value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    _Adjust_heap(first, long(0), long(last - first), value, comp);
}
// std::__make_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp, SKeyedEvents*, long*) -- SGI/STLport make_heap
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC070, 114 bytes, `ret 0xc` (/Gr).
// Label correction: catalog "std_heapify_BuildHeap" -> std::__make_heap (SGI form).
// Callees: CSmallVector::Copy 0x006FB5C0, __adjust_heap 0x006FBD60.
// Shape: SGI `if (last - first < 2) return; len; parent = (len - 2) / 2; for (;;) { __adjust_heap(first, parent, len,
//   T(*(first + parent)), comp); if (parent == 0) return; parent--; }`.
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

void __fastcall __make_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp, SKeyedEvents*, long*)
{
    if (last - first < 2)
        return;
    long len = last - first;
    long parent = (len - 2) / 2;
    for (;;) {
        _Adjust_heap(first, parent, len, SKeyedEvents(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
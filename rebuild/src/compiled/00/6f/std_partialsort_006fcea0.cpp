// std::__partial_sort(SKeyedEvents* first, SKeyedEvents* middle, SKeyedEvents* last, SKeyedEvents*, CKeyPairCompareLess comp)
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCEA0, 111 bytes, `ret 0xc` (/Gr).
// Label correction: catalog "AnimEventMap_MakeHeapSort" -> std::__partial_sort (SGI form) of the keyed event vector.
// Callees: __make_heap 0x006FC070, CSmallVector::Copy 0x006FB5C0, __pop_heap 0x006FBFD0, sort_heap 0x006FCB90.
// Shape: SGI `make_heap; for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first, middle, i, T(*i),
//   comp, distance_type); sort_heap;`.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

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

void __fastcall __make_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp, SKeyedEvents*, long*);   // 0x006FC070
void __fastcall __pop_heap(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents* result, SKeyedEvents value,
                           CKeyPairCompareLess comp, long*);                                                          // 0x006FBFD0
void __fastcall _Sort_heap(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess pred);                        // 0x006FCB90

void __fastcall __partial_sort(SKeyedEvents* first, SKeyedEvents* middle, SKeyedEvents* last, SKeyedEvents*,
                               CKeyPairCompareLess comp)
{
    __make_heap(first, middle, comp, (SKeyedEvents*)0, (long*)0);
    for (SKeyedEvents* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SKeyedEvents(*i), comp, (long*)0);
    _Sort_heap(first, middle, comp);
}
// std::__insertion_sort(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCB30, 81 bytes, `ret 4` (/Gr).
// Label correction: catalog "AnimEvent_DeleteAll" -> std::__insertion_sort of the keyed event vector.
// Callees: CSmallVector::Copy 0x006FB5C0 (the value copy), __linear_insert 0x006FC3E0.
// Shape: `for (i = first + 1; i != last; ++i) __linear_insert(first, i, T(*i), comp);` with the copy built in the
//   argument slot.
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

void __fastcall __linear_insert(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp); // 0x006FC3E0

void __fastcall __insertion_sort(SKeyedEvents* first, SKeyedEvents* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyedEvents* i = first + 1; i != last; ++i)
        __linear_insert(first, i, SKeyedEvents(*i), comp);
}
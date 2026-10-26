// std::__unguarded_partition(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents pivot, CKeyPairCompareLess comp)
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FBEE0, 190 bytes, `ret 0x10` (/Gr: first, last in ecx/edx; pivot by value).
// Label correction: catalog "std_quicksort_Partition" -> std::__unguarded_partition of the keyed event vector.
// Callees: CSmallVector::Copy 0x006FB5C0 (assignments), operator delete[] 0x00BFEB1C (temporaries' destructors).
// Shape: SGI loop `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if
//   (!(first < last)) return first; iter_swap(first, last); ++first; }` with an inline swap through a named temporary
//   and the CSmallVector `Clear()` destructor.
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

template<class T> inline void __iter_swap(T* a, T* b)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

SKeyedEvents* __fastcall __unguarded_partition(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents pivot,
                                               CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        __iter_swap(first, last);
        ++first;
    }
}
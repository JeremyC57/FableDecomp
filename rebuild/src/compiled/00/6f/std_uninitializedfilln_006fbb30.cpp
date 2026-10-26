// std::__uninitialized_fill_n(SKeyedEvents* first, unsigned int n, const SKeyedEvents& x, const __false_type&) -- copy-construct n pairs
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FBB30, 63 bytes, `ret 8` (/Gr).
// Label correction: catalog "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is propagated; this is
//   std::__uninitialized_fill_n of pair<unsigned long, CSmallVector<CEvent,32> >.
// Callees: CSmallVector::Copy 0x006FB5C0 (the copy constructor's body).
// Shape: placement copy-construction: the key, then `Start = 0, Size = 0` and Copy(other) (an unsigned count).
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

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

struct __false_type { __false_type() {} };

template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

SKeyedEvents* __fastcall __uninitialized_fill_n(SKeyedEvents* first, unsigned int n, const SKeyedEvents& x, const __false_type&)
{
    SKeyedEvents* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
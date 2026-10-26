// std::__destroy_aux(SKeyedEvents* first, SKeyedEvents* last, const __false_type&) -- destroy each pair
// Leaf: __destroy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FB8A0, 47 bytes, `ret 4` (/Gr).
// Label correction: catalog "_Copy_backward<CPixelFormat*,CPixelFormat*>" is a propagated twin label; this is
//   std::__destroy_aux over pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > (the AnimToExtraEventsMap
//   element).
// Callees: operator delete[] 0x00BFEB1C.
// Shape: the inlined CSmallVector destructor (Clear: delete[] Start, zero Start and Size) per element.
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

struct __false_type { __false_type() {} };

template<class T> inline void _Destroy(T* pointer) { pointer->~T(); }

void __fastcall __destroy_aux(SKeyedEvents* first, SKeyedEvents* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
// std::vector<SKeyedEvents>::push_back(const SKeyedEvents& x) -- construct at finish, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FCD90, 82 bytes, `ret 4`.
// Label correction: catalog "AnimEventVector_PushBack" -> STLport vector<pair<unsigned long, CSmallVector<CEvent,32> >
//   >::push_back.
// Callees: CSmallVector::Copy 0x006FB5C0, _M_insert_overflow 0x006FC6A0 (same-TU noinline STAND-IN body so the empty
//   tag shares x's slot).
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

template<class T> class allocator { public: allocator() {} };

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
#pragma auto_inline(off)
    // 0x006FC6A0 (separate row; same-TU stand-in so the empty tag can share x's slot)
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        if (atend && fill_len == 1 && position == _M_finish)
            _Construct(_M_start, x);
    }
#pragma auto_inline(on)
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

template void vector<SKeyedEvents>::push_back(const SKeyedEvents& x);
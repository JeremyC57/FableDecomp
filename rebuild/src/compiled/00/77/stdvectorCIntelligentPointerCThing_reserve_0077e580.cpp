// std::vector<CIntelligentPointer<CThing> >::reserve(unsigned int n) -- STLport reserve for a vector of intelligent thing pointers: reallocate to n slots when the capacity is smaller, copying the elements over and destroying the old block
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E580, 151 bytes, thiscall `ret 4`.
// Callees: malloc @ 0x00BFEA0E (inline allocator, both arms), __uninitialized_copy(first, last, result, const
//   __false_type&) @ 0x004AA140 (`__fastcall`: the CIntelligentPointer copy loop -- vtable 0x01238C6C, GetPItem/SetPItem),
//   _M_clear() @ 0x004AC1D0 (virtual destructor of each 8-byte element, then free).
// Callers: CTCBurntEffect::CTCBurntEffect 0x007CDFE0 (landed: names this address `vector<CIntelligentPointer<CThing> >
//   ::reserve` for TintList), 0x0077D5B0, 0x007CEB.., 0x007CF1.., 0x008E8060.
// Label correction: catalogued as `vector_reserve_CConversation`; the element is CIntelligentPointer<CThing>.
// Shape: byte twin (immediates masked) of the landed vector<pair<CCharString,long> >::reserve 0x006D4680: STLport 4.x
//   reserve with the member-template _M_allocate_and_copy inlined (allocate + the out-of-line copy), the old size
//   computed first (ebp), the cached finish spilled to the `push ecx` frame word; the `__false_type` tag has a user
//   constructor and the copy loop has a same-TU noinline body, so VC7.1 homes the tag over the live `n` argument slot.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CThing* PItem;                                     // +0x04
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C, 8 bytes
    CIntelligentPointer(const CIntelligentPointer& other);
};

namespace std {
struct __false_type { __false_type() { } };

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

// Same-TU body of the out-of-line copy (0x004AA140), non-template, noinline: VC7.1 then sees that the empty tag is never
// read and homes it over the live `n` argument slot (`lea eax,[esp+18h]`) as retail does.
#pragma auto_inline(off)
__declspec(noinline) CIntelligentPointer<CThing>* __fastcall __uninitialized_copy(CIntelligentPointer<CThing>* first,
    CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>* result, const __false_type&)   // 0x004AA140
{
    CIntelligentPointer<CThing>* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CIntelligentPointer<CThing>(*first);
    return cur;
}
#pragma auto_inline(on)

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    typedef unsigned int size_type;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    template <class FI> T* _M_allocate_and_copy(size_type n, FI first, FI last)
    {
        T* result = _M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void _M_clear();                                            // 0x004AC1D0
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void reserve(size_type n);
};

template <class T, class A> void vector<T, A>::reserve(size_type n)
{
    if (capacity() < n) {
        const size_type old_size = size();
        T* tmp;
        if (_M_start) {
            tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
            _M_clear();
        } else {
            tmp = _M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void vector<CIntelligentPointer<CThing> >::reserve(unsigned int);
}
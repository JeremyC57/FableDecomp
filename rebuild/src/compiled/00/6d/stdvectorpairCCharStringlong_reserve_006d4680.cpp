// std::vector<pair<CCharString,long> >::reserve(unsigned int n) -- STLport reserve for a vector of (CCharString, long) pairs: reallocate to n slots when the capacity is smaller, copying the elements over and destroying the old ones
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4680, length 151 bytes, thiscall `ret 4`. Single caller:
// CThingNPC::TransferLoveState (call at 0x006D3024). The manifest label "CCombatWheel_ReserveRings" is descriptive and
// wrong: the element is the 8-byte pair<CCharString, long> (the copy loop 0x006D3770 calls CCharString's copy ctor
// 0x0099EC30, _M_clear 0x006D45C0 runs ~CCharString 0x0099EAE0 over [start, finish) before freeing).
// Callees: malloc @ 0x00BFEA0E (inline allocator, both arms), __uninitialized_copy(first, last, result, const
// __false_type&) @ 0x006D3770 (`__fastcall`, landed as UninitCopy_SPairCSecond_0099ec30Long), _M_clear() @ 0x006D45C0.
// Shape: STLport 4.x reserve with the member-template _M_allocate_and_copy inlined (allocate + the out-of-line copy),
// the old size computed first (ebp) and the cached finish spilled to the `push ecx` frame word. Two levers: the
// `__false_type` tag needs a user constructor (else `mov byte ptr [..],0`), and the copy needs its real body in the TU
// (non-template, noinline, under auto_inline(off)) -- only then does VC7.1 home the tag over the live `n` argument slot
// (`lea eax,[esp+18h]`) instead of a frame byte.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> struct allocator {
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

// Same-TU body of the out-of-line copy (the landed 0x006D3770 loop), non-template, noinline: VC7.1 then sees that the
// empty tag is never read and homes it over the live `n` argument slot (`lea eax,[esp+18h]`) as retail does.
#pragma auto_inline(off)
__declspec(noinline) pair<CCharString, long>* __fastcall __uninitialized_copy(pair<CCharString, long>* first,
    pair<CCharString, long>* last, pair<CCharString, long>* result, const __false_type&)   // 0x006D3770
{
    pair<CCharString, long>* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) pair<CCharString, long>(*first);
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
    void _M_clear();                                            // 0x006D45C0
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

template void vector<pair<CCharString, long> >::reserve(unsigned int);
}
// std::vector<CScriptThing>::reserve(size_type n) -- STLport reserve of the 12-byte script-thing handles: when capacity() < n, allocate n slots, copy the elements over and clear the old block (or just allocate when empty), keep the size
// Leaf: ?reserve@?$vector@VCScriptThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ACD30, 201 bytes, thiscall, `ret 4`.
// Identity: the manifest's "std::vector::_Reserve" is descriptive. The landed __uninitialized_copy 0x008AC140 (CScriptThing,
//   12 bytes: vptr 0x01238C8C + CCountedPointer<CScriptThing> PImp) lists 0x008ACDA8 in this body as a caller; the five
//   callers are the CGameScriptInterface GetAllThingsWith* / GetAllCreatures* queries 0x008A8570..0x008A9C40, which
//   reserve their result vector<CScriptThing>.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x008AC140 (`__fastcall`,
//   the tag by reference), vector::_M_clear() 0x008ACA50 (out of line: each element's virtual destructor, then the
//   null-tested free; the manifest's "~vector<CConversation>" is propagated).
// Shape: STLport 4.6 vector.c `reserve(n) { if (capacity() < n) { const size_type old_size = size(); pointer tmp; if
//   (_M_start) { tmp = _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); } else tmp =
//   _M_end_of_storage.allocate(n); _M_set(tmp, tmp + old_size, tmp + n); } }` with _M_allocate_and_copy inline (its
//   result spilled to the `push ecx` word across the copy). The copy gets the address of the LIVE `n` argument slot as
//   its tag: a same-TU noinline __uninitialized_copy stand-in that never touches the tag.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo { public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void*);                   // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CScriptThing : public CBaseClass {                    // vtable 0x01238C8C, 12 bytes
public:
    CCountedPointer<CScriptThing> PImp;                     // +0x04
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

// Same-TU stand-in for 0x008AC140 (its own landed row), kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }

    protected:
    void _M_clear();                                        // out of line (0x008ACA50)
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
    void reserve(size_type __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, (const_pointer)this->_M_start, (const_pointer)this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<CScriptThing>::reserve(unsigned int);

} // namespace std
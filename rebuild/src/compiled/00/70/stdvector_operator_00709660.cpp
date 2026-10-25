// std::vector::operator=(const vector<vector<CModifierEntry> >& x) -- assign the hero's appearance-modifier vector of vectors
// Leaf: ??4?$vector@V?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00709660, 276 bytes, thiscall `ret 4`, returns this.
// Label correction: catalog "CFeatDef_VectorReassign" -> STLport 4.x
//   vector<vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry> >::operator= (the landed
//   CTCHeroAttachableAppearanceModifiers::Copy 0x00706EC0 assigns AppearanceModifiers through it).
// Callees: _M_allocate_and_copy 0x007092B0, _M_clear 0x00709510, __copy 0x00709090 (each inner vector assigned through
//   its out-of-line operator= 0x00708CA0), _Destroy(first, last) 0x007094D0, __uninitialized_copy 0x00709030.
// Shape: STLport 4.6 `operator=` verbatim: `if (&x != this)`, xlen = x.size(); grow (allocate-and-copy, _M_clear,
//   new start / end of storage), shrink (__copy_ptrs -> __copy with a random_access_iterator_tag and a null distance
//   pointer, then _Destroy of the surplus) or extend (__copy over size() elements, __uninitialized_copy of the rest);
//   `_M_finish = _M_start + xlen` is tail-duplicated into each arm. x._M_finish is cached in x's dead argument slot
//   and the empty tags are homed on that same slot: the tag-taking callees are same-TU noinline stand-ins that never
//   touch the tag (their real bodies are separate rows).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CTCHeroAttachableAppearanceModifiers {
public:
    class CModifierEntry {                      // Ego_r.pdb, 8 bytes
    public:
        long DefIndex;                          // +0x00
        bool Rendered;                          // +0x04
    };
};
typedef CTCHeroAttachableAppearanceModifiers::CModifierEntry CModifierEntry;

namespace std {

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template<class T> class vector;

// the inner vector<CModifierEntry> (12 bytes): only what the outer assignment uses
template<> class vector<CModifierEntry> {
public:
    CModifierEntry* _M_start;                   // +0x00
    CModifierEntry* _M_finish;                  // +0x04
    CModifierEntry* _M_end_of_storage;          // +0x08
    vector(const vector& x);
    vector& operator=(const vector& x);         // 0x00708CA0
    ~vector() { if (_M_start) free(_M_start); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x00709090, 0x007094D0, 0x00709030).
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class T> __declspec(noinline) void __fastcall _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
#pragma auto_inline(on)

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template<class T> class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    allocator<T> _M_alloc() const { return allocator<T>(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
#pragma auto_inline(off)
    // 0x007092B0 (stand-in)
    __declspec(noinline) T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = _M_alloc().allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    // 0x00709510 (stand-in)
    __declspec(noinline) void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, capacity());
    }
#pragma auto_inline(on)
    vector& operator=(const vector& x);
};

template<class T>
vector<T>& vector<T>::operator=(const vector<T>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            T* __tmp = _M_allocate_and_copy(__xlen, (const T*)__x._M_start + 0, (const T*)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            T* __i = __copy_ptrs((const T*)__x._M_start + 0, (const T*)__x._M_finish + 0, (T*)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const T*)__x._M_start, (const T*)__x._M_start + size(), (T*)this->_M_start, __false_type());
            __uninitialized_copy((const T*)__x._M_start + size(), (const T*)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template class vector<vector<CModifierEntry> >;

}
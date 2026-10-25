// std::vector::_M_insert_overflow(CIntelligentPointer<CThing>* position, const CIntelligentPointer<CThing>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow an intelligent-pointer vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739550, 263 bytes, `ret 0x14`.
// Label correction: catalog "Vector_Reallocate_IntelligentPointer" -> STLport 4.x
//   vector<CIntelligentPointer<T> >::_M_insert_overflow (callers: the inline push_back 0x007398B0 of the CTCCombat TU and
//   0x008788AA). 8-byte element (`sar eax,3`, `lea [ecx*8]`); T is not recoverable from the body (every
//   CIntelligentPointer<T> shares vtable 0x01237F1C), CThing is a descriptive choice.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00737750, __uninitialized_fill_n 0x007377A0,
//   CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::GetPItem 0x00A01B50, ::SetPItem 0x00A01B90.
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0): len = size + max(size, fill_len) via the const-ref `max`,
//   allocate, copy the head, construct one element in place (the inlined CIntelligentPointer copy constructor: base
//   ctor, vtable store, SetPItem(x.GetPItem())) or fill_n, copy the tail unless atend; _M_clear's destroy loop is
//   inlined here (each element's virtual destructor through vtable slot 0 with flag 0), then free and _M_set. The
//   copy / fill helpers are same-TU noinline stand-ins (their real bodies are separate rows).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CThing;

class CBaseIntelligentPointer {
public:
    void* PData;                                // +0x04 (CBaseObjectPointer*)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // slot 0
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(const void* item);            // 0x00A01B90
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual ~CIntelligentPointer() {}           // vtable 0x01237F1C
};

namespace std {

struct __false_type { __false_type() {} };

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x00737750, 0x007377A0).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
#pragma auto_inline(on)

template<class T> inline void __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(first);
}
template<class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<CIntelligentPointer<CThing> >;

}
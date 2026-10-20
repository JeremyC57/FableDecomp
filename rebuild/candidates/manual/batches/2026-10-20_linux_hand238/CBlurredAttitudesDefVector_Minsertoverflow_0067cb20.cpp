// CBlurredAttitudesDefVector::_M_insert_overflow(CBlurredAttitudesDef*, const CBlurredAttitudesDef&, const __false_type&, unsigned long, bool) -- STLport vector<CBlurredAttitudesDef> regrowth: allocate max(size, n) more, copy the head, construct the n new elements, copy the tail unless appending, destroy (virtually) and free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067CB20, 257 bytes, `ret 14h`.
// Identity: the manifest row "CVectorBase::resize_realloc" is descriptive; the body is the STLport
//   vector<CBlurredAttitudesDef>::_M_insert_overflow (0x48-byte elements, `imul 38E38E39h ; sar 4`; its single-element
//   copy is the CBlurredAttitudesDef copy ctor 0x0067C6D0 and its fill helper the landed _Uninit_fill_n<CBlurredAttitudesDef*>
//   0x0067CA70; GOTCHAS chunk 177 lists 0x0067D610 as this vector's resize(n)).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the node allocator), __fastcall __uninitialized_copy 0x0067C9F0 (x2),
//   CBlurredAttitudesDef(const CBlurredAttitudesDef&) 0x0067C6D0, __fastcall __uninitialized_fill_n 0x0067CA70, the
//   element's virtual destructor (slot 0, flag 0) in the inline _Destroy loop.
// Shape: the landed vector<SEntityScriptEntry>::_M_insert_overflow (0x00CBC850) idiom: `(max)(old_size, fill_len)` through
//   const references (old size in the push-ecx word), allocate `n != 0 ? malloc(n * 0x48) : 0` homed in the dead
//   fill_len slot, the tags in the atend slot; same-TU stand-in bodies for the two fastcall helpers keep the frame.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CBlurredAttitudesDef {                          // 0x48 bytes (vtable 0x0125B8AC)
    virtual ~CBlurredAttitudesDef();
    unsigned char _body[0x44];
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);   // 0x0067C6D0
};
void* FindRef_free___uninitialized_copy_2();          // stand-in callee of the stand-in body below
void* FindRef_free___uninitialized_fill_n_1();        // stand-in callee of the stand-in body below
// Stand-in bodies (the real ones are out of line in retail, 0x0067C9F0 / 0x0067CA70): they only write a word through
// their pointer parameters inline, so VC7.1 knows the caller's address-taken locals do not escape the calls (precedent:
// the landed vector<SEntityScriptEntry>::_M_insert_overflow 0x00CBC850).
#pragma auto_inline(off)
__declspec(noinline) CBlurredAttitudesDef* __fastcall __uninitialized_copy(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last,
                                                                           CBlurredAttitudesDef* result, const __false_type& p3)   // 0x0067C9F0
{
    void* p = FindRef_free___uninitialized_copy_2();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
__declspec(noinline) CBlurredAttitudesDef* __fastcall __uninitialized_fill_n(CBlurredAttitudesDef* first, unsigned long n,
                                                                             const CBlurredAttitudesDef& x, const __false_type& p3)   // 0x0067CA70
{
    void* p = FindRef_free___uninitialized_fill_n_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)&x = p;
    return 0;
}
#pragma auto_inline(on)
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
struct CBlurredAttitudesDefVector {                    // STLport vector (12 bytes in retail)
    CBlurredAttitudesDef* _M_start;
    CBlurredAttitudesDef* _M_finish;
    CBlurredAttitudesDef* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static CBlurredAttitudesDef* allocate(unsigned long n)
    {
        return n != 0 ? (CBlurredAttitudesDef*)malloc(n * sizeof(CBlurredAttitudesDef)) : 0;
    }
    static void deallocate(CBlurredAttitudesDef* p)
    {
        if (p)
            free(p);
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        deallocate(_M_start);
    }
    void _M_set(CBlurredAttitudesDef* s, CBlurredAttitudesDef* f, CBlurredAttitudesDef* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(CBlurredAttitudesDef* position, const CBlurredAttitudesDef& x, const __false_type&,
                            unsigned long fill_len, bool atend);
};
void CBlurredAttitudesDefVector::_M_insert_overflow(CBlurredAttitudesDef* position, const CBlurredAttitudesDef& x,
                                                    const __false_type&, unsigned long fill_len, bool atend)
{
    const unsigned long old_size = size();
    const unsigned long len = old_size + (max)(old_size, fill_len);
    CBlurredAttitudesDef* new_start = allocate(len);
    CBlurredAttitudesDef* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
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

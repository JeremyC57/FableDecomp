// CActiveEntityScriptVector::_M_insert_overflow(SEntityScriptEntry* position, const SEntityScriptEntry& x, const __false_type&, unsigned long fill_len, bool atend) -- STLport vector<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > regrowth: allocate max(size, n) more, copy the head, construct the n new elements, copy the tail unless appending, destroy and free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC850, 230 bytes (padded to 240), `ret 14h`. The catalog label
//   "CVectorMap_EntryScriptBase::AllocateGrowth" is descriptive; the body is the STLport member (callers 0x00CB8A8E,
//   0x00CBD304, 0x00CBD634 are push_back/insert paths of the script base's active-entity-script vector map).
// Callees (all `/Gr` SGI-style free templates, `__fastcall`): __uninitialized_copy 0x00CBA3E0 (first ecx, last edx,
//   result + tag on the stack), __uninitialized_fill_n 0x00CBA420 (first ecx, n edx, x + tag on the stack), the range
//   destroy 0x00CBB1B0 (first ecx, last edx, tag); CRT malloc 0x00BFEA0E / free 0x00BFEA14 (the node allocator).
// Shape: `(max)(old_size, fill_len)` through const references (`old_size` spilled to the push-ecx frame word,
//   `lea ecx` of the winner); allocate is `n != 0 ? malloc(n * 16) : 0`; the single-element case is the inlined
//   placement copy of the pair (memberwise: two key words, the counted pointer with its add-ref); every
//   `__false_type()` tag temporary is homed in the `atend` argument slot.
// Element: pair<unsigned __int64 key, CCountedPointer<CActiveEntityScriptBase>> (16 bytes: key +0, Data +8, Info +0xc;
//   the count block's reference count is its first word).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
class CActiveEntityScriptBase;
struct SCountBlock { long RefCount; };
struct CCountedPointer_CActiveEntityScriptBase {
    CActiveEntityScriptBase* Data;                     // +0
    SCountBlock* Info;                                 // +4
    CCountedPointer_CActiveEntityScriptBase(const CCountedPointer_CActiveEntityScriptBase& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
struct SEntityScriptEntry {                            // pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
    unsigned long KeyLow;                              // +0
    unsigned long KeyHigh;                             // +4
    CCountedPointer_CActiveEntityScriptBase Script;    // +8
    SEntityScriptEntry(const SEntityScriptEntry& other)
        : KeyLow(other.KeyLow), KeyHigh(other.KeyHigh), Script(other.Script) { }
};
SEntityScriptEntry* __fastcall __uninitialized_copy(SEntityScriptEntry* first, SEntityScriptEntry* last,
                                                    SEntityScriptEntry* result, const __false_type&);    // 0x00CBA3E0
void* FindRef_free___uninitialized_copy_2();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) SEntityScriptEntry* __fastcall __uninitialized_copy(SEntityScriptEntry* first, SEntityScriptEntry* last, SEntityScriptEntry* result, const __false_type& p3)
{
    void* p = FindRef_free___uninitialized_copy_2();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
#pragma auto_inline(on)
SEntityScriptEntry* __fastcall __uninitialized_fill_n(SEntityScriptEntry* first, unsigned long n,
                                                      const SEntityScriptEntry& x, const __false_type&); // 0x00CBA420
void* FindRef_free___uninitialized_fill_n_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) SEntityScriptEntry* __fastcall __uninitialized_fill_n(SEntityScriptEntry* first, unsigned long n, const SEntityScriptEntry& x, const __false_type& p3)
{
    void* p = FindRef_free___uninitialized_fill_n_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)&x = p;
    return 0;
}
#pragma auto_inline(on)
void __fastcall _Destroy(SEntityScriptEntry* first, SEntityScriptEntry* last, const __false_type&);   // 0x00CBB1B0
void* FindRef_free__Destroy_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void __fastcall _Destroy(SEntityScriptEntry* first, SEntityScriptEntry* last, const __false_type& p2)
{
    void* p = FindRef_free__Destroy_0();
    if (!p)
        return;
    *(void**)first = p;
    *(void**)last = p;
}
#pragma auto_inline(on)
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
struct CActiveEntityScriptVector {                     // STLport vector (12 bytes in retail)
    SEntityScriptEntry* _M_start;
    SEntityScriptEntry* _M_finish;
    SEntityScriptEntry* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static SEntityScriptEntry* allocate(unsigned long n)
    {
        return n != 0 ? (SEntityScriptEntry*)malloc(n * sizeof(SEntityScriptEntry)) : 0;
    }
    static void deallocate(SEntityScriptEntry* p)
    {
        if (p)
            free(p);
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish, __false_type());
        deallocate(_M_start);
    }
    void _M_set(SEntityScriptEntry* s, SEntityScriptEntry* f, SEntityScriptEntry* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(SEntityScriptEntry* position, const SEntityScriptEntry& x, const __false_type&,
                            unsigned long fill_len, bool atend);
};
void CActiveEntityScriptVector::_M_insert_overflow(SEntityScriptEntry* position, const SEntityScriptEntry& x,
                                                   const __false_type&, unsigned long fill_len, bool atend)
{
    const unsigned long old_size = size();
    const unsigned long len = old_size + (max)(old_size, fill_len);
    SEntityScriptEntry* new_start = allocate(len);
    SEntityScriptEntry* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
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
// CActiveFile::AllocateAndInitializeCategoryNodes(CCategory*, const CCategory&, const __false_type&, unsigned long,
// bool) -- the catalog's name for the STLport vector<CCategory>::_M_insert_overflow(pos, x, tag, fillLength, atEnd):
// grow to old + max(old, fillLength) elements, copy the head, construct the inserted copies, copy the tail unless
// appending, free the old block and adopt the new one.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2C65, length 177 bytes, `ret 14h`, size-optimised TU with a frame.
// Callees: malloc @ 0x00BFEA0E (0x2c-byte elements), __uninitialized_copy @ 0x005C2929 (__fastcall, twice),
// CCategory::CCategory(const CCategory&) @ 0x005C2086 (placement copy, null-tested), __uninitialized_fill_n
// @ 0x005C2954 (__fastcall), the vector's _M_clear @ 0x005C2C42 (landed under a propagated "OnReadFinished" label).
// Shape: STLport 4.x source; `(max)(old, fill)` through references (`lea ecx,&fill ; cmp ; jb ; lea ecx,&old`),
// allocate's `n ? malloc(n * 0x2c) : 0` reusing the old-size slot; the __false_type temporaries sit in the high byte
// of the atEnd argument slot (`lea eax,[ebp+1Bh]`); _M_set stores end of storage, finish, start.
// Flags: #pragma optimize("y", off) + ("s", on) (frame pointer and `leave`, size TU).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };     // user ctor: the tag temporaries take no stores

struct CCategory {                                  // 0x2c bytes
    unsigned char _body[0x2c];
    CCategory(const CCategory& other);              // 0x005C2086
};

CCategory* __fastcall UninitializedCopy(const CCategory* first, const CCategory* last, CCategory* result,
                                        const __false_type& isPOD);          // 0x005C2929
void* FindRef_free_UninitializedCopy_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CCategory* __fastcall UninitializedCopy(const CCategory* first, const CCategory* last, CCategory* result, const __false_type& isPOD)
{
    void* p = FindRef_free_UninitializedCopy_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
#pragma auto_inline(on)
CCategory* __fastcall UninitializedFillN(CCategory* first, unsigned long count, const CCategory& value,
                                         const __false_type& isPOD);         // 0x005C2954
void* FindRef_free_UninitializedFillN_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CCategory* __fastcall UninitializedFillN(CCategory* first, unsigned long count, const CCategory& value, const __false_type& isPOD)
{
    void* p = FindRef_free_UninitializedFillN_0();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)&value = p;
    return 0;
}
#pragma auto_inline(on)

template<class T> __forceinline const T& Max(const T& a, const T& b) { return a < b ? b : a; }   // STLport (max)

struct CCategoryVector {                            // STLport vector<CCategory>: start, finish, end of storage
    CCategory* _M_start;
    CCategory* _M_finish;
    CCategory* _M_end_of_storage;
    __forceinline unsigned long size() const { return _M_finish - _M_start; }
    static __forceinline CCategory* allocate(unsigned long n) { return n != 0 ? (CCategory*)malloc(n * sizeof(CCategory)) : 0; }
    void _M_clear();                                // 0x005C2C42
    __forceinline void _M_set(CCategory* start, CCategory* finish, CCategory* endOfStorage)
    {
        _M_start = start;
        _M_finish = finish;
        _M_end_of_storage = endOfStorage;
    }
    void _M_insert_overflow(CCategory* position, const CCategory& x, const __false_type& isPOD,
                            unsigned long fillLength, bool atEnd);
};

void CCategoryVector::_M_insert_overflow(CCategory* position, const CCategory& x, const __false_type&,
                                         unsigned long fillLength, bool atEnd)
{
    const unsigned long oldSize = size();
    const unsigned long length = oldSize + Max(oldSize, fillLength);
    CCategory* newStart = allocate(length);
    CCategory* newFinish = UninitializedCopy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        new(newFinish) CCategory(x);
        ++newFinish;
    } else {
        newFinish = UninitializedFillN(newFinish, fillLength, x, __false_type());
    }
    if (!atEnd)
        newFinish = UninitializedCopy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
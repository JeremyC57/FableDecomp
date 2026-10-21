// std::vector<SSymbolEntry>::_M_fill_insert(iterator pos, size_type n, const SSymbolEntry& x) -- STLport insert of n copies: in place (shift the tail up by n, or fill past the end first) when the capacity allows, else through _M_insert_overflow
// Leaf: _M_fill_insert
// Retail 0x0052C300, 346 bytes, `ret 0Ch`. Label: catalogued as "CSymbolMap::Insert"; the body is the STLport 4.x
// vector<T>::_M_fill_insert instantiation for a 24-byte element of the symbol-map TU (CCharString name, a POD vector,
// a long and a flag; descriptive name SSymbolEntry).
// Callees: CCharString copy ctor 0x0099EC30, the POD vector's copy ctor 0x005290E0 (malloc + memmove), ~CCharString 0x0099EAE0,
// free 0x00BFEA14 (the copy's inline vector dtor); `__fastcall` STLport helpers: __uninitialized_copy(first, last, result,
// const __false_type&) 0x0052BBE0, __copy_backward(first, last, result, const random_access_iterator_tag&, int*)
// 0x0052B640, fill(first, last, const T&) 0x0052BCA0, __uninitialized_fill_n(first, n, const T&, const __false_type&)
// 0x0052B510; the member _M_insert_overflow(pos, x, const __false_type&, n, bool) 0x0052C150.
// Shape: the STLport text with the vector<CCombatWheel::SSlot>::operator= model (landed 0x006F3C20): same-TU stand-ins
// for the out-of-line helpers (auto_inline(off), above the caller) keep every tag temporary in the dead `n` slot.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);
};
struct CLongVector {                                       // vector<POD> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector(const CLongVector& other);                 // 0x005290E0
    ~CLongVector() { if (_M_start) free(_M_start); }
    CLongVector& operator=(const CLongVector& other);
};
struct SSymbolEntry {                                      // 0x18 bytes
    CCharString Name;                                      // +0x00
    CLongVector Values;                                    // +0x04
    long        Value;                                     // +0x10
    bool        Flag;                                      // +0x14
};

void ConstructEntry(SSymbolEntry* p, const SSymbolEntry& x);
#pragma auto_inline(off)
SSymbolEntry* __fastcall __uninitialized_copy(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry* result, const __false_type&)   // 0x0052BBE0
{
    for (; first != last; ++first, ++result)
        ConstructEntry(result, *first);
    return result;
}
SSymbolEntry* __fastcall __copy_backward(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry* result,
                                         const random_access_iterator_tag&, int*)   // 0x0052B640
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
void __fastcall fill(SSymbolEntry* first, SSymbolEntry* last, const SSymbolEntry& value)   // 0x0052BCA0
{
    for (; first != last; ++first)
        *first = value;
}
SSymbolEntry* __fastcall __uninitialized_fill_n(SSymbolEntry* first, unsigned int n, const SSymbolEntry& x, const __false_type&)   // 0x0052B510
{
    for (; n > 0; --n, ++first)
        ConstructEntry(first, x);
    return first;
}
#pragma auto_inline(on)
inline SSymbolEntry* __copy_backward_ptrs(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline SSymbolEntry* uninitialized_fill_n(SSymbolEntry* first, unsigned int n, const SSymbolEntry& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

struct CSymbolEntryVector {                                // STLport vector<SSymbolEntry> (12 bytes)
    typedef unsigned int size_type;
    SSymbolEntry* _M_start;
    SSymbolEntry* _M_finish;
    SSymbolEntry* _M_end_of_storage;
    void _M_insert_overflow(SSymbolEntry* pos, const SSymbolEntry& x, const __false_type&, size_type fill_len, bool atend = false);   // 0x0052C150
    void _M_fill_insert(SSymbolEntry* pos, size_type n, const SSymbolEntry& x);
};

void CSymbolEntryVector::_M_fill_insert(SSymbolEntry* pos, size_type n, const SSymbolEntry& x)
{
    if (n != 0) {
        if (size_type(_M_end_of_storage - _M_finish) >= n) {
            SSymbolEntry x_copy = x;
            const size_type elems_after = _M_finish - pos;
            SSymbolEntry* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(pos, old_finish - n, old_finish, __false_type());
                fill(pos, pos + n, x_copy);
            } else {
                uninitialized_fill_n(_M_finish, n - elems_after, x_copy);
                _M_finish += n - elems_after;
                __uninitialized_copy(pos, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                fill(pos, old_finish, x_copy);
            }
        } else
            _M_insert_overflow(pos, x, __false_type(), n);
    }
}

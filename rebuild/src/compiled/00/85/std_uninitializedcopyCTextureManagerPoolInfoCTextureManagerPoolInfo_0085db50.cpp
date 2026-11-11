// _global::UninitCopy_Record20_0085db50(const SRecord20* first, const SRecord20* last, SRecord20* result, const __false_type&) -- placement copy-construct [first, last) of 20-byte records (memberwise copy constructor) into raw storage, return the advanced result
// Leaf: UninitCopy_Record20_0085db50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085DB50, length 62 bytes (`ret 8`).
// Identity: catalog `_Move<NThingMap::CMapwhoLevelInfo*>` is propagated (placement copy, not an assignment). The landed
//   vector _M_fill_insert 0x0085E570 of the same TU models this 20-byte element with an explicit word copy. Callers: the
//   reallocating insert 0x0085E200 and the fill-insert 0x0085E570 (two calls each).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   An explicit memberwise copy constructor copies straight through ecx/eax with esi as the only temporary
//   (`push esi` only); the implicit POD copy gives the `mov esi,ecx ; mov edi,eax` + ebx form of 0x0081A650 (tried).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord20 {                              // 20-byte record with an explicit memberwise copy constructor
    long m_0, m_4, m_8, m_c, m_10;
    SRecord20(const SRecord20& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10) { }
};

SRecord20* __fastcall UninitCopy_Record20_0085db50(const SRecord20* first, const SRecord20* last, SRecord20* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord20(*first);
    return result;
}
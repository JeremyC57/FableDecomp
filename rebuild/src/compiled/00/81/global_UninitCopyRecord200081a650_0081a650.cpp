// _global::UninitCopy_Record20_0081a650(const SRecord20* first, const SRecord20* last, SRecord20* result, const __false_type&) -- placement copy-construct [first, last) of 20-byte POD records into raw storage, return the advanced result
// Leaf: UninitCopy_Record20_0081a650
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081A650, length 68 bytes (`ret 8`).
// Identity: catalog `_Move<NThingMap::CMapwhoLevelInfo*>`: the body is a null-tested placement copy (not an assignment) of
//   20-byte records; NThingMap::CMapwhoLevelInfo is 20 bytes in Ego_r.pdb but nothing here pins the element to it, so the
//   record stays descriptive. Caller: the vector fill-insert helpers 0x0081C270 / 0x0081D0B0 (two calls each).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack, `ret 8`); the placement new keeps the `test eax,eax` null test on the
//   destination and the advanced result is returned.
//   The implicit (bitwise) copy constructor of a 5-word POD is copied through reference registers: `mov esi,ecx ;
//   mov edi,eax` and five ebx moves (an explicit memberwise copy constructor gives the direct [ecx+N] form of
//   0x0085DB50 instead; 9 words or more become `rep movsd`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord20 {                              // 20-byte POD record, implicit copy constructor
    long m_0, m_4, m_8, m_c, m_10;
};

SRecord20* __fastcall UninitCopy_Record20_0081a650(const SRecord20* first, const SRecord20* last, SRecord20* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord20(*first);
    return result;
}
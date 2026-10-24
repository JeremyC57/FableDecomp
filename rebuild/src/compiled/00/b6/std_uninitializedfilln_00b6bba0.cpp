// std::__uninitialized_fill_n(CLandscapeRecord*, unsigned int, const CLandscapeRecord&) -- placement-copy one 0x38-byte trivially copyable record into each of n slots and return the end
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x00B6BBA0, 43 bytes, `ret 4`: ecx = first, edx = n, the value on the stack; returns first + n in eax.
//   Called once, from the vector<record>::_M_fill_insert at 0x00B6C700 region (call site 0x00B6C7CC; catalog neighbours
//   Vector_InsertMultipleFill_Struct0x38 / Vector_Resize_Struct0x38).
// Callees: none (the element copy is an inline 14-dword rep movsd).
// Shape: STLport's `for (; n > 0; --n, ++first) _Construct(first, x)`: unsigned `test edx,edx ; jbe`, the null-checked placement
//   copy inside the loop, the cursor kept in eax. The element type is not recoverable from the catalog label (CAppearanceModifierGraphics::
//   CEntry is propagated and is 0x28 bytes in the PDB); modelled as an opaque 0x38-byte record.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00b6bba0 {
struct CLandscapeRecord {                      // 0x38 bytes, trivially copyable
    unsigned long Words[14];
};
CLandscapeRecord* __fastcall __uninitialized_fill_n(CLandscapeRecord* first, unsigned int n, const CLandscapeRecord& x)
{
    for (; n > 0; --n, ++first)
        if (first != 0)
            new (first) CLandscapeRecord(x);   // _Construct
    return first;
}
}
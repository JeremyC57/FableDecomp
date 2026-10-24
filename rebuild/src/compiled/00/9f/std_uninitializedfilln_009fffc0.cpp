// std::__uninitialized_fill_n(CGraphicBankRecord*, unsigned int, const CGraphicBankRecord&) -- placement-copy one 0x2C-byte trivially copyable record into each of n slots and return the end
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x009FFFC0, 43 bytes, `ret 4`: ecx = first, edx = n, the value on the stack; returns first + n in eax.
//   Called once, from the vector<record>::_M_fill_insert at 0x00A01290 (CGraphicDataBank's TU; stride imul 0x2C).
// Callees: none (the element copy is an inline 11-dword rep movsd).
// Shape: STLport's `for (; n > 0; --n, ++first) _Construct(first, x)`: unsigned `test edx,edx ; jbe`, the null-checked placement
//   copy inside the loop, the cursor kept in eax. The element type is not recoverable from the catalog label (CAppearanceModifierGraphics::
//   CEntry is propagated and is 0x28 bytes in the PDB); modelled as an opaque 0x2C-byte record.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N009fffc0 {
struct CGraphicBankRecord {                      // 0x2C bytes, trivially copyable
    unsigned long Words[11];
};
CGraphicBankRecord* __fastcall __uninitialized_fill_n(CGraphicBankRecord* first, unsigned int n, const CGraphicBankRecord& x)
{
    for (; n > 0; --n, ++first)
        if (first != 0)
            new (first) CGraphicBankRecord(x);   // _Construct
    return first;
}
}
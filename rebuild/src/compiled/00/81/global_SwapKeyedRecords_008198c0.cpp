// SwapKeyedRecords(SKeyedRecord24& a, SKeyedRecord24& b) -- std::swap-style: exchange two 24-byte keyed records (a key word plus a 20-byte value) through a temporary
// Leaf: SwapKeyedRecords
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008198C0, length 113 bytes, `__fastcall` (a in ecx, b in edx).
// Label: the catalog's CPreTransposedBoneMatrix::Set is propagated; the body is a two-reference swap reached only
// through the `jmp` thunk at 0x0081A8A0 (an STL algorithm helper's instantiation; the element type is not named by
// any evidence here, so the record is modelled descriptively).
// Shape: `T tmp = a; a = b; b = tmp;` -- the temporary is built by the record's memberwise user copy constructor
// (pair-style: the key word, then the POD value through its IMPLICIT copy, addressed with `lea esi,[ecx+4] ; mov
// ebx,esi`); a user copy constructor on the value folds the address away. Both assignments are implicit.
struct SValue20 {                               // the 20-byte value
    long w0, w1, w2, w3, w4;
    // implicit copy constructor
};

struct SKeyedRecord24 {
    unsigned long key;                          // +0x00
    SValue20      value;                        // +0x04
    SKeyedRecord24(const SKeyedRecord24& o) : key(o.key), value(o.value) {}
};

void __fastcall SwapKeyedRecords(SKeyedRecord24& a, SKeyedRecord24& b)
{
    SKeyedRecord24 tmp = a;
    a = b;
    b = tmp;
}
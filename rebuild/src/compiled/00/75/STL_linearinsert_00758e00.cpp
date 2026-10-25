// std::__linear_insert(SKeyedRecord2C* first, SKeyedRecord2C* last, SKeyedRecord2C val, CKeyCompareLess comp) -- insertion-sort step over 0x2C-byte records keyed by a signed long: a value smaller than the first element shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00758E00, 77 bytes, `ret 30h` (__fastcall: first in ecx, last in edx, the
// 0x2C-byte value and the empty comparator on the stack).
// Callees: 0x00758C50 std::copy_backward(first, last, result) (__fastcall, out of line, `ret 4`; catalogued as the
// propagated `_Copy_backward<pair<unsigned long,C3DDummyObject>*>`), 0x00758390 std::__unguarded_linear_insert(last,
// val, comp) (__fastcall; catalogued as the propagated `_Copy_impl<CDegradableInfo*>`). Both are referenced only here.
// Identity: the manifest's `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize` is propagated; no code
// references this body (an out-of-line COMDAT of an insertion-sort helper, the 0x2C-element sibling of 0x00A33800).
// The record type is not recoverable beyond its size and the signed long key at +0 (`cmp ; jge`), so it is modelled
// as { long key; 40 bytes }.
// Shape: copy_backward stays a call here (it is out of line in this module); `*first = val` is an 11-dword
// `rep movsd` (implicit operator=); the by-value argument copy in the else arm is memberwise -- the key already
// loaded for the compare (`mov [eax],ecx`) then 10 dwords -- so the record has a user copy constructor.
namespace std {

struct SRecordBody28 { long Words[10]; };

struct SKeyedRecord2C {                                // 0x2C bytes
    long          Key;                                 // +0x00
    SRecordBody28 Body;                                // +0x04
    SKeyedRecord2C(const SKeyedRecord2C& r) : Key(r.Key), Body(r.Body) { }
};

struct CKeyCompareLess {
    bool operator()(const SKeyedRecord2C& a, const SKeyedRecord2C& b) const { return a.Key < b.Key; }
};

SKeyedRecord2C* __fastcall copy_backward(SKeyedRecord2C* first, SKeyedRecord2C* last, SKeyedRecord2C* result);   // 0x00758C50
void __fastcall __unguarded_linear_insert(SKeyedRecord2C* last, SKeyedRecord2C val, CKeyCompareLess comp);       // 0x00758390

void __fastcall __linear_insert(SKeyedRecord2C* first, SKeyedRecord2C* last, SKeyedRecord2C val, CKeyCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}
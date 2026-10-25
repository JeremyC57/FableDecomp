// std::__linear_insert(SKeyedRecord28* first, SKeyedRecord28* last, SKeyedRecord28 val, CKeyCompareLess comp) -- insertion-sort step over 0x28-byte records keyed by a signed long: a value smaller than the first element shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081BE30, 77 bytes (+3 int3 padding), `ret 2Ch` (__fastcall: first in ecx, last in edx, the
// 0x28-byte value and the empty comparator on the stack).
// Callees: 0x0081BA10 std::copy_backward(first, last, result) (__fastcall, out of line, `ret 4`; catalogued as the
// propagated `_Copy_backward<pair<unsigned long,C3DDummyObject>*>`), 0x008199F0 std::__unguarded_linear_insert(last,
// val, comp) (__fastcall; catalogued as the propagated `_Copy_impl<CDegradableInfo*>`). Both are referenced only here.
// Identity: the manifest's `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize` is propagated; no code
// references this body (an out-of-line COMDAT of an insertion-sort helper; the 0x28-element sibling of 0x00758E00 / 0x00A33800).
// The record type is not recoverable beyond its size and the signed long key at +0 (`cmp ; jge`), so it is modelled
// as { long key; 36 bytes }.
// Shape: copy_backward stays a call here (it is out of line in this module); `*first = val` is a 10-dword
// `rep movsd` (implicit operator=); the by-value argument copy in the else arm is memberwise -- the key already
// loaded for the compare (`mov [eax],ecx`) then 9 dwords -- so the record has a user copy constructor.
namespace std {

struct SRecordBody24 { long Words[9]; };

struct SKeyedRecord28 {                                // 0x28 bytes
    long          Key;                                 // +0x00
    SRecordBody24 Body;                                // +0x04
    SKeyedRecord28(const SKeyedRecord28& r) : Key(r.Key), Body(r.Body) { }
};

struct CKeyCompareLess {
    bool operator()(const SKeyedRecord28& a, const SKeyedRecord28& b) const { return a.Key < b.Key; }
};

SKeyedRecord28* __fastcall copy_backward(SKeyedRecord28* first, SKeyedRecord28* last, SKeyedRecord28* result);   // 0x0081BA10
void __fastcall __unguarded_linear_insert(SKeyedRecord28* last, SKeyedRecord28 val, CKeyCompareLess comp);       // 0x008199F0

void __fastcall __linear_insert(SKeyedRecord28* first, SKeyedRecord28* last, SKeyedRecord28 val, CKeyCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}
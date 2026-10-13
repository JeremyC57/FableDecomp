// CRetailVector_CElement36_008e2fc0::push_back(const CElement36_008e2fc0*) -- placement-copy at `last` when there is room (36-byte elements, copy ctor @ 0x008E2FC0), else InsertGrow @ 0x008E3AA0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4360, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CElement36_008e2fc0::CElement36_008e2fc0(const CElement36_008e2fc0&) ; S: add dword ptr [esi+4],0x24 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 36-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x008E2FC0 and `last` advances; when full, the reallocating
// insert @ 0x008E3AA0 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 17 bodies.
#include <new>

struct CElement36_008e2fc0 {
    unsigned char _body[0x24];
    CElement36_008e2fc0(const CElement36_008e2fc0& other);           // 0x008E2FC0
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CElement36_008e2fc0 {
    CElement36_008e2fc0* first;
    CElement36_008e2fc0* last;
    CElement36_008e2fc0* end;
    void InsertGrow(CElement36_008e2fc0* where, const CElement36_008e2fc0& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x008E3AA0
    void PushBack_CElement36_008e2fc0_008e4360(const CElement36_008e2fc0& value);
};

void CRetailVector_CElement36_008e2fc0::PushBack_CElement36_008e2fc0_008e4360(const CElement36_008e2fc0& value)
{
    if (last != end) {
        new (last) CElement36_008e2fc0(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
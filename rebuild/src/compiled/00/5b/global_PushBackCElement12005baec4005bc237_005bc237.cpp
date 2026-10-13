#pragma optimize("s",on)
// CRetailVector_CElement12_005baec4::push_back(const CElement12_005baec4*) -- placement-copy at `last` when there is room (12-byte elements, copy ctor @ 0x005BAEC4), else InsertGrow @ 0x005BC0FB
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC237, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CElement12_005baec4::CElement12_005baec4(const CElement12_005baec4&) ; S: add dword ptr [esi+4],0xc ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 12-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x005BAEC4 and `last` advances; when full, the reallocating
// insert @ 0x005BC0FB is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CElement12_005baec4 {
    unsigned char _body[0xc];
    CElement12_005baec4(const CElement12_005baec4& other);           // 0x005BAEC4
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CElement12_005baec4 {
    CElement12_005baec4* first;
    CElement12_005baec4* last;
    CElement12_005baec4* end;
    void InsertGrow(CElement12_005baec4* where, const CElement12_005baec4& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x005BC0FB
    void PushBack_CElement12_005baec4_005bc237(const CElement12_005baec4& value);
};

void CRetailVector_CElement12_005baec4::PushBack_CElement12_005baec4_005bc237(const CElement12_005baec4& value)
{
    if (last != end) {
        new (last) CElement12_005baec4(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
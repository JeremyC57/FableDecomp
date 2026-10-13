#pragma optimize("s",on)
// CRetailVector_CElement16_004935f8::push_back(const CElement16_004935f8*) -- placement-copy at `last` when there is room (16-byte elements, copy ctor @ 0x004935F8), else InsertGrow @ 0x00493FF8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049417D, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CElement16_004935f8::CElement16_004935f8(const CElement16_004935f8&) ; S: add dword ptr [esi+4],0x10 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 16-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x004935F8 and `last` advances; when full, the reallocating
// insert @ 0x00493FF8 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CElement16_004935f8 {
    unsigned char _body[0x10];
    CElement16_004935f8(const CElement16_004935f8& other);           // 0x004935F8
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CElement16_004935f8 {
    CElement16_004935f8* first;
    CElement16_004935f8* last;
    CElement16_004935f8* end;
    void InsertGrow(CElement16_004935f8* where, const CElement16_004935f8& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00493FF8
    void PushBack_CElement16_004935f8_0049417d(const CElement16_004935f8& value);
};

void CRetailVector_CElement16_004935f8::PushBack_CElement16_004935f8_0049417d(const CElement16_004935f8& value)
{
    if (last != end) {
        new (last) CElement16_004935f8(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
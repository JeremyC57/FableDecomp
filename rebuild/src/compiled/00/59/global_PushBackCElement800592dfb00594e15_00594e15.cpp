#pragma optimize("s",on)
// CRetailVector_CElement8_00592dfb::push_back(const CElement8_00592dfb*) -- placement-copy at `last` when there is room (8-byte elements, copy ctor @ 0x00592DFB), else InsertGrow @ 0x00594D36
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594E15, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CElement8_00592dfb::CElement8_00592dfb(const CElement8_00592dfb&) ; S: add dword ptr [esi+4],0x8 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 8-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x00592DFB and `last` advances; when full, the reallocating
// insert @ 0x00594D36 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CElement8_00592dfb {
    unsigned char _body[0x8];
    CElement8_00592dfb(const CElement8_00592dfb& other);           // 0x00592DFB
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CElement8_00592dfb {
    CElement8_00592dfb* first;
    CElement8_00592dfb* last;
    CElement8_00592dfb* end;
    void InsertGrow(CElement8_00592dfb* where, const CElement8_00592dfb& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00594D36
    void PushBack_CElement8_00592dfb_00594e15(const CElement8_00592dfb& value);
};

void CRetailVector_CElement8_00592dfb::PushBack_CElement8_00592dfb_00594e15(const CElement8_00592dfb& value)
{
    if (last != end) {
        new (last) CElement8_00592dfb(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
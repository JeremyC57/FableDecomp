// CRetailVector_CElement12_00a87090::push_back(const CElement12_00a87090*) -- placement-copy at `last` when there is room (12-byte elements, copy ctor @ 0x00A87090), else InsertGrow @ 0x00F3F890
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FAA0, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CElement12_00a87090::CElement12_00a87090(const CElement12_00a87090&) ; S: add dword ptr [esi+4],0xc ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 12-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x00A87090 and `last` advances; when full, the reallocating
// insert @ 0x00F3F890 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 11 bodies.
#include <new>

struct CElement12_00a87090 {
    unsigned char _body[0xc];
    CElement12_00a87090(const CElement12_00a87090& other);           // 0x00A87090
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CElement12_00a87090 {
    CElement12_00a87090* first;
    CElement12_00a87090* last;
    CElement12_00a87090* end;
    void InsertGrow(CElement12_00a87090* where, const CElement12_00a87090& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00F3F890
    void PushBack_CElement12_00a87090_00f3faa0(const CElement12_00a87090& value);
};

void CRetailVector_CElement12_00a87090::PushBack_CElement12_00a87090_00f3faa0(const CElement12_00a87090& value)
{
    if (last != end) {
        new (last) CElement12_00a87090(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
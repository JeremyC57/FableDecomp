// CRetailVector_CRegion::push_back(const CRegion*) -- placement-copy at `last` when there is room (88-byte elements, copy ctor @ 0x0051A900), else InsertGrow @ 0x0051CF10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D200, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CRegion::CRegion(const CRegion&) ; S: add dword ptr [esi+4],0x58 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 88-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x0051A900 and `last` advances; when full, the reallocating
// insert @ 0x0051CF10 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 17 bodies.
#include <new>

struct CRegion {
    unsigned char _body[0x58];
    CRegion(const CRegion& other);           // 0x0051A900
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CRegion {
    CRegion* first;
    CRegion* last;
    CRegion* end;
    void InsertGrow(CRegion* where, const CRegion& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0051CF10
    void PushBack_CRegion_0051d200(const CRegion& value);
};

void CRetailVector_CRegion::PushBack_CRegion_0051d200(const CRegion& value)
{
    if (last != end) {
        new (last) CRegion(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

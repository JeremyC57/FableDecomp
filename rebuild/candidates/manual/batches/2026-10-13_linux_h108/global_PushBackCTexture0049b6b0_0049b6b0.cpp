// CRetailVector_CTexture::push_back(const CTexture*) -- placement-copy at `last` when there is room (8-byte elements, copy ctor @ 0x009F9D00), else InsertGrow @ 0x0049B480
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B6B0, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CTexture::CTexture(const CTexture&) ; S: add dword ptr [esi+4],0x8 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 8-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x009F9D00 and `last` advances; when full, the reallocating
// insert @ 0x0049B480 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 11 bodies.
#include <new>

struct CTexture {
    unsigned char _body[0x8];
    CTexture(const CTexture& other);           // 0x009F9D00
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CTexture {
    CTexture* first;
    CTexture* last;
    CTexture* end;
    void InsertGrow(CTexture* where, const CTexture& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0049B480
    void PushBack_CTexture_0049b6b0(const CTexture& value);
};

void CRetailVector_CTexture::PushBack_CTexture_0049b6b0(const CTexture& value)
{
    if (last != end) {
        new (last) CTexture(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

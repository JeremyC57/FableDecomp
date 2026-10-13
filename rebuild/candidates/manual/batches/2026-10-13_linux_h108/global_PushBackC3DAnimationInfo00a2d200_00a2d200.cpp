// CRetailVector_C3DAnimationInfo::push_back(const C3DAnimationInfo*) -- placement-copy at `last` when there is room (40-byte elements, copy ctor @ 0x00A2A4F0), else InsertGrow @ 0x00A2CAC0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D200, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call C3DAnimationInfo::C3DAnimationInfo(const C3DAnimationInfo&) ; S: add dword ptr [esi+4],0x28 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 40-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x00A2A4F0 and `last` advances; when full, the reallocating
// insert @ 0x00A2CAC0 is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 11 bodies.
#include <new>

struct C3DAnimationInfo {
    unsigned char _body[0x28];
    C3DAnimationInfo(const C3DAnimationInfo& other);           // 0x00A2A4F0
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_C3DAnimationInfo {
    C3DAnimationInfo* first;
    C3DAnimationInfo* last;
    C3DAnimationInfo* end;
    void InsertGrow(C3DAnimationInfo* where, const C3DAnimationInfo& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00A2CAC0
    void PushBack_C3DAnimationInfo_00a2d200(const C3DAnimationInfo& value);
};

void CRetailVector_C3DAnimationInfo::PushBack_C3DAnimationInfo_00a2d200(const C3DAnimationInfo& value)
{
    if (last != end) {
        new (last) C3DAnimationInfo(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

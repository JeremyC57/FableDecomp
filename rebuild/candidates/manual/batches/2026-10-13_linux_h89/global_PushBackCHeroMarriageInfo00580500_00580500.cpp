// CRetailVector_CHeroMarriageInfo::push_back(const CHeroMarriageInfo*) -- placement-copy at `last` when there is room (24-byte elements, copy ctor @ 0x0057A3A2), else InsertGrow @ 0x0058044A
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00580500, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call CHeroMarriageInfo::CHeroMarriageInfo(const CHeroMarriageInfo&) ; S: add dword ptr [esi+4],0x18 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call InsertGrow ; pop esi ; ret 4
// Retail vector push_back for 24-byte elements {first, last, end}: with spare capacity the element
// is copy-constructed in place at `last` (placement new, hence the null test) through the
// out-of-line copy constructor @ 0x0057A3A2 and `last` advances; when full, the reallocating
// insert @ 0x0058044A is called for one element with `noTail` set. Its third argument is a
// const reference to an empty tag object (Dinkumware-style iterator category): the compiler homes
// that 1-byte temporary in this function's dead argument slot, which is the address retail pushes.
// The element type is named from the copy constructor's label when unique. One of 12 bodies.
#include <new>

struct CHeroMarriageInfo {
    unsigned char _body[0x18];
    CHeroMarriageInfo(const CHeroMarriageInfo& other);           // 0x0057A3A2
};

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary                 // empty iterator-category tag: its 1-byte temporary lives in a dead stack slot

struct CRetailVector_CHeroMarriageInfo {
    CHeroMarriageInfo* first;
    CHeroMarriageInfo* last;
    CHeroMarriageInfo* end;
    void InsertGrow(CHeroMarriageInfo* where, const CHeroMarriageInfo& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0058044A
    void PushBack_CHeroMarriageInfo_00580500(const CHeroMarriageInfo& value);
};

void CRetailVector_CHeroMarriageInfo::PushBack_CHeroMarriageInfo_00580500(const CHeroMarriageInfo& value)
{
    if (last != end) {
        new (last) CHeroMarriageInfo(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}

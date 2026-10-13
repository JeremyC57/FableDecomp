// std::find<long*> -- Dinkumware `_Find`: four-way unrolled scan with a switch on the remainder (4-byte elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00612DB0, oracle length 112 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; mov ecx,edx ; push ebx ; sub ecx,eax ; push esi ; sar ecx,4 ; test ecx,ecx ; push edi ; mov edi,[esp+10h] ; jle T ; mov esi,[edi] ; L: cmp [eax],esi ; je F ; (three more compare/advance steps) ; add eax,4 ; dec ecx ; test ecx,ecx ; jg L ; T: mov ecx,edx ; sub ecx,eax ; sar ecx,2 ; dec ecx ; je C1 ; dec ecx ; je C2 ; dec ecx ; jne D ; (case 3, 2, 1 compares) ; D: mov eax,edx ; F: pop edi ; pop esi ; pop ebx ; ret 8
// The retail Dinkumware `find` for 4-byte scalar elements (__fastcall: first in ecx, last in
// edx, the value by reference on the stack; `ret 8` also pops an unused tag): a loop over groups
// of four comparisons (the value cached in a register) followed by a `switch` on the 0..3
// remaining elements. Written out by hand because the DDK headers are a different revision.
// One of 23 bodies of this shape.
struct CIteratorTag { };

long* __fastcall Find_00612db0(long* first, long* last, const long& value, CIteratorTag tag)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}

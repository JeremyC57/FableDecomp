// SGI __copy_backward<SElement12*> (12-byte elements) -- counted backward copy, returns the moved-back result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819720, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; test eax,eax ; jle Z ; push ebx ; mov ecx,eax ; mov eax,[result] ; push edi ; L: sub esi,0xc ; sub eax,0xc ; dec ecx ; (block copy of 12 bytes from [esi] to [eax]) ; jne L ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` for
// 12-byte elements: `for (n = last - first; n > 0; --n) *--result = *--last;` and the moved-back
// result is returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the
// stack, `ret 0Ch`). The POD copy is a block move through pointer registers. The manifest
// label (`_Move_backward` / `_Copy_backward`) is propagated from Dinkumware. One of 9 bodies.
struct SElement12 {
    unsigned char _body[0xc];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement12* __fastcall CopyBackward_SElement12_00819720(SElement12* first, SElement12* last, SElement12* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
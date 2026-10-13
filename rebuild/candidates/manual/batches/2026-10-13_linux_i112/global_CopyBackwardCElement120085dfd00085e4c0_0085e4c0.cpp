// SGI __copy_backward<CElement12_0085dfd0*> (12-byte elements, out-of-line operator= @ 0x0085DFD0) -- counted backward copy, returns the moved-back result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085E4C0, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: sub esi,0xc ; sub edi,0xc ; push esi ; mov ecx,edi ; call operator= ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` for
// 12-byte elements: `for (n = last - first; n > 0; --n) *--result = *--last;` and the moved-back
// result is returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the
// stack, `ret 0Ch`). Each element is assigned through the out-of-line operator= @ 0x0085DFD0. The manifest
// label (`_Move_backward` / `_Copy_backward`) is propagated from Dinkumware. One of 9 bodies.
struct CElement12_0085dfd0 {
    unsigned char _body[0xc];
    CElement12_0085dfd0& operator=(const CElement12_0085dfd0& other);   // 0x0085DFD0
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement12_0085dfd0* __fastcall CopyBackward_CElement12_0085dfd0_0085e4c0(CElement12_0085dfd0* first, CElement12_0085dfd0* last, CElement12_0085dfd0* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

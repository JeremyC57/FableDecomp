// SGI __copy_backward<CElement12_00f3b640*> (12-byte elements, out-of-line operator= @ 0x00F3B640) -- counted backward copy, returns the moved-back result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B8F0, oracle length 66 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; test eax,eax ; jle Z ; push ebx ; push edi ; mov edi,[result] ; mov ebx,eax ; L: sub esi,0xc ; sub edi,0xc ; push esi ; mov ecx,edi ; call operator= ; dec ebx ; jne L ; mov eax,edi ; pop edi ; pop ebx ; pop esi ; ret 0Ch ; Z: mov eax,[result] ; pop esi ; ret 0Ch
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` for
// 12-byte elements: `for (n = last - first; n > 0; --n) *--result = *--last;` and the moved-back
// result is returned. /Gr TU (__fastcall: first in ecx, last in edx, result + two tags on the
// stack, `ret 0Ch`). Each element is assigned through the out-of-line operator= @ 0x00F3B640. The manifest
// label (`_Move_backward` / `_Copy_backward`) is propagated from Dinkumware. One of 9 bodies.
struct CElement12_00f3b640 {
    unsigned char _body[0xc];
    CElement12_00f3b640& operator=(const CElement12_00f3b640& other);   // 0x00F3B640
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement12_00f3b640* __fastcall CopyBackward_CElement12_00f3b640_00f3b8f0(CElement12_00f3b640* first, CElement12_00f3b640* last, CElement12_00f3b640* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

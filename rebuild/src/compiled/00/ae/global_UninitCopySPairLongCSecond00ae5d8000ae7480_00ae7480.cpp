// _Uninit_copy<SPairLongCSecond_00ae5d80*> -- placement-copy [first, last) to dest (16-byte {long, CSecond_00ae5d80} pairs, CSecond_00ae5d80 copy ctor @ 0x00AE5D80), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7480, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call CSecond_00ae5d80::CSecond_00ae5d80 ; S: add esi,0x10 ; add edi,0x10 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose first member is a 4-byte scalar and whose second
// member CSecond_00ae5d80 has the out-of-line copy constructor @ 0x00AE5D80: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_00ae5d80 {
    unsigned char _body[0xc];
    CSecond_00ae5d80(const CSecond_00ae5d80& other);           // 0x00AE5D80
};

struct SPairLongCSecond_00ae5d80 {
    long first;
    CSecond_00ae5d80 second;
};

struct CRetailAllocator { };

SPairLongCSecond_00ae5d80* __fastcall UninitCopy_SPairLongCSecond_00ae5d80_00ae7480(SPairLongCSecond_00ae5d80* first, SPairLongCSecond_00ae5d80* last, SPairLongCSecond_00ae5d80* dest, CRetailAllocator allocator)
{
    SPairLongCSecond_00ae5d80* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairLongCSecond_00ae5d80(*first);
    return d;
}
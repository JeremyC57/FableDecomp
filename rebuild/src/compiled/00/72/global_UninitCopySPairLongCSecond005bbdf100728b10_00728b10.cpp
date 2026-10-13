// _Uninit_copy<SPairLongCSecond_005bbdf1*> -- placement-copy [first, last) to dest (16-byte {long, CSecond_005bbdf1} pairs, CSecond_005bbdf1 copy ctor @ 0x005BBDF1), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728B10, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call CSecond_005bbdf1::CSecond_005bbdf1 ; S: add esi,0x10 ; add edi,0x10 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose first member is a 4-byte scalar and whose second
// member CSecond_005bbdf1 has the out-of-line copy constructor @ 0x005BBDF1: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_005bbdf1 {
    unsigned char _body[0xc];
    CSecond_005bbdf1(const CSecond_005bbdf1& other);           // 0x005BBDF1
};

struct SPairLongCSecond_005bbdf1 {
    long first;
    CSecond_005bbdf1 second;
};

struct CRetailAllocator { };

SPairLongCSecond_005bbdf1* __fastcall UninitCopy_SPairLongCSecond_005bbdf1_00728b10(SPairLongCSecond_005bbdf1* first, SPairLongCSecond_005bbdf1* last, SPairLongCSecond_005bbdf1* dest, CRetailAllocator allocator)
{
    SPairLongCSecond_005bbdf1* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairLongCSecond_005bbdf1(*first);
    return d;
}
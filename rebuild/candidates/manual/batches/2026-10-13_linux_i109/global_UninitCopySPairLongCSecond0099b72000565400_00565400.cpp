// _Uninit_copy<SPairLongCSecond_0099b720*> -- placement-copy [first, last) to dest (8-byte {long, CSecond_0099b720} pairs, CSecond_0099b720 copy ctor @ 0x0099B720), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00565400, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call CSecond_0099b720::CSecond_0099b720 ; S: add esi,0x8 ; add edi,0x8 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose first member is a 4-byte scalar and whose second
// member CSecond_0099b720 has the out-of-line copy constructor @ 0x0099B720: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_0099b720 {
    unsigned char _body[0x4];
    CSecond_0099b720(const CSecond_0099b720& other);           // 0x0099B720
};

struct SPairLongCSecond_0099b720 {
    long first;
    CSecond_0099b720 second;
};

struct CRetailAllocator { };

SPairLongCSecond_0099b720* __fastcall UninitCopy_SPairLongCSecond_0099b720_00565400(SPairLongCSecond_0099b720* first, SPairLongCSecond_0099b720* last, SPairLongCSecond_0099b720* dest, CRetailAllocator allocator)
{
    SPairLongCSecond_0099b720* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairLongCSecond_0099b720(*first);
    return d;
}

// _Uninit_copy<SPairCSecond_0099ec30Long*> -- placement-copy [first, last) to dest (8-byte {CSecond_0099ec30, long} pairs, CSecond_0099ec30 copy ctor @ 0x0099EC30), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B2680, oracle length 52 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CSecond_0099ec30::CSecond_0099ec30 ; mov eax,[esi+4] ; mov [edi+4],eax ; S: add esi,0x8 ; add edi,0x8 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose second member is a 4-byte scalar and whose first
// member CSecond_0099ec30 has the out-of-line copy constructor @ 0x0099EC30: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_0099ec30 {
    unsigned char _body[0x4];
    CSecond_0099ec30(const CSecond_0099ec30& other);           // 0x0099EC30
};

struct SPairCSecond_0099ec30Long {
    CSecond_0099ec30 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_0099ec30Long* __fastcall UninitCopy_SPairCSecond_0099ec30Long_009b2680(SPairCSecond_0099ec30Long* first, SPairCSecond_0099ec30Long* last, SPairCSecond_0099ec30Long* dest, CRetailAllocator allocator)
{
    SPairCSecond_0099ec30Long* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairCSecond_0099ec30Long(*first);
    return d;
}

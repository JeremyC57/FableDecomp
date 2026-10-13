// _Uninit_copy<SPairLongCSecond_0099ec30*> -- placement-copy [first, last) to dest (8-byte {long, CSecond_0099ec30} pairs, CSecond_0099ec30 copy ctor @ 0x0099EC30), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CF950, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; mov eax,[esi] ; lea ecx,[esi+4] ; push ecx ; lea ecx,[edi+4] ; mov [edi],eax ; call CSecond_0099ec30::CSecond_0099ec30 ; S: add esi,0x8 ; add edi,0x8 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose first member is a 4-byte scalar and whose second
// member CSecond_0099ec30 has the out-of-line copy constructor @ 0x0099EC30: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_0099ec30 {
    unsigned char _body[0x4];
    CSecond_0099ec30(const CSecond_0099ec30& other);           // 0x0099EC30
};

struct SPairLongCSecond_0099ec30 {
    long first;
    CSecond_0099ec30 second;
};

struct CRetailAllocator { };

SPairLongCSecond_0099ec30* __fastcall UninitCopy_SPairLongCSecond_0099ec30_004cf950(SPairLongCSecond_0099ec30* first, SPairLongCSecond_0099ec30* last, SPairLongCSecond_0099ec30* dest, CRetailAllocator allocator)
{
    SPairLongCSecond_0099ec30* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairLongCSecond_0099ec30(*first);
    return d;
}
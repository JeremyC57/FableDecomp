// _Uninit_copy<SPairCSecond_00673e10Long*> -- placement-copy [first, last) to dest (16-byte {CSecond_00673e10, long} pairs, CSecond_00673e10 copy ctor @ 0x00673E10), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D530, oracle length 52 bytes.
// Grade: candidate -- still DIFFER(52v52) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CSecond_00673e10::CSecond_00673e10 ; mov eax,[esi+4] ; mov [edi+4],eax ; S: add esi,0x10 ; add edi,0x10 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose second member is a 4-byte scalar and whose first
// member CSecond_00673e10 has the out-of-line copy constructor @ 0x00673E10: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_00673e10 {
    unsigned char _body[0xc];
    CSecond_00673e10(const CSecond_00673e10& other);           // 0x00673E10
};

struct SPairCSecond_00673e10Long {
    CSecond_00673e10 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_00673e10Long* __fastcall UninitCopy_SPairCSecond_00673e10Long_0086d530(SPairCSecond_00673e10Long* first, SPairCSecond_00673e10Long* last, SPairCSecond_00673e10Long* dest, CRetailAllocator allocator)
{
    SPairCSecond_00673e10Long* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairCSecond_00673e10Long(*first);
    return d;
}

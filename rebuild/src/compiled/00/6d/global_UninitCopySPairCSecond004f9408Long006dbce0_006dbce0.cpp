// _Uninit_copy<SPairCSecond_004f9408Long*> -- placement-copy [first, last) to dest (16-byte {CSecond_004f9408, long} pairs, CSecond_004f9408 copy ctor @ 0x004F9408), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBCE0, oracle length 52 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[dest] ; je E ; L: test edi,edi ; je S ; push esi ; mov ecx,edi ; call CSecond_004f9408::CSecond_004f9408 ; mov eax,[esi+4] ; mov [edi+4],eax ; S: add esi,0x10 ; add edi,0x10 ; cmp esi,ebx ; jne L ; E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Dinkumware `_Uninit_copy` (`__fastcall`: first in ecx, last in edx, dest and the by-value
// allocator on the stack) for a pair whose second member is a 4-byte scalar and whose first
// member CSecond_004f9408 has the out-of-line copy constructor @ 0x004F9408: the pair's implicit copy
// constructor is inlined into the placement new (hence the null test) and the advanced
// destination is returned. One of 9 bodies of this shape.
#include <new>

struct CSecond_004f9408 {
    unsigned char _body[0xc];
    CSecond_004f9408(const CSecond_004f9408& other);           // 0x004F9408
};

struct SPairCSecond_004f9408Long {
    CSecond_004f9408 first;
    long second;
};

struct CRetailAllocator { };

SPairCSecond_004f9408Long* __fastcall UninitCopy_SPairCSecond_004f9408Long_006dbce0(SPairCSecond_004f9408Long* first, SPairCSecond_004f9408Long* last, SPairCSecond_004f9408Long* dest, CRetailAllocator allocator)
{
    SPairCSecond_004f9408Long* d = dest;
    for (; first != last; ++first, ++d)
        new (d) SPairCSecond_004f9408Long(*first);
    return d;
}
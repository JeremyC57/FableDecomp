// _Uninit_fill_n<SPairLongLong*> -- placement-copy `count` copies of an 8-byte {long, long} pair, return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39380, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; mov ecx,[value] ; push esi ; L: test eax,eax ; je S ; mov esi,[ecx] ; mov [eax],esi ; mov esi,[ecx+4] ; mov [eax+4],esi ; S: add eax,8 ; dec edx ; jne L ; pop esi ; E: ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// value reference and the by-value allocator on the stack) for an 8-byte pair of 4-byte scalars:
// the trivial copy is two dword moves under the placement-new null test; the advanced
// destination is returned. One of 15 bodies.
#include <new>

struct SPairLongLong {
    long first;
    long second;
};

struct CRetailAllocator { };

SPairLongLong* __fastcall UninitFillN_SPairLongLong_00a39380(SPairLongLong* first, unsigned long count, const SPairLongLong& value)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongLong(value);
    return first;
}
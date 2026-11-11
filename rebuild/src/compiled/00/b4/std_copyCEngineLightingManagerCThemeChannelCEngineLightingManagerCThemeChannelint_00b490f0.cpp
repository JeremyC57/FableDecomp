// SGI __copy<SElement112*> (random-access, 112-byte POD elements) -- counted forward copy, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B490F0, oracle length 72 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub edx,ecx ; (divide by 112) ; test eax,eax ; jle Z ; push ebx ; push esi ; mov edx,eax ; mov eax,[result] ; push edi ; L: (copy 112 bytes from [ecx] to [eax]) ; add ecx,0x70 ; add eax,0x70 ; dec edx ; jne L ; pop edi ; pop esi ; pop ebx ; ret 0Ch ; Z: mov eax,[result] ; ret 0Ch
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` for 112-byte
// trivially-copyable elements: the signed element count is computed once (the compiler's
// reciprocal multiply for the division by 112), the loop copies one element per iteration
// (`*result = *first; ++first; ++result;`) and the advanced result is returned (the untouched
// result for an empty range). The TU is compiled with /Gr (all free functions __fastcall):
// first in ecx, last in edx, result + the two tag arguments on the stack (`ret 0Ch`). The
// manifest label (`_Copy_impl<...>`) is propagated from Dinkumware; the body is SGI's.
// One of 12 bodies of this shape.
struct SElement112 {
    unsigned char _body[0x70];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement112* __fastcall Copy_SElement112_00b490f0(SElement112* first, SElement112* last, SElement112* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
// _Uninit_fill_n<T*> -- `count` copies of value constructed through the __fastcall Construct @ 0x0045BB8F (4-byte elements, size TU), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00469149, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe R ; push edi ; mov edi,edx ; mov edx,[value] ; mov ecx,esi ; call Construct ; add esi,4 ; dec edi ; jne L ; pop edi ; R: mov eax,esi ; pop esi ; ret 4
// Dinkumware `_Uninit_fill_n` where the element's construction stays out of line: the __fastcall
// `_Construct` @ 0x0045BB8F takes the slot in ecx and the value in edx; the count runs down in
// edi; the size TU keeps the loop unaligned. The manifest label is propagated; the element is
// a 4-byte opaque class. One of 5 bodies of this shape.
#pragma optimize("s", on)

struct CElement { long m_value; };
void __fastcall Construct(CElement* where, const CElement& value);   // 0x0045BB8F

CElement* __fastcall UninitFillN_00469149(CElement* first, unsigned long count, const CElement& value)
{
    for (; 0 < count; --count, ++first)
        Construct(first, value);
    return first;
}

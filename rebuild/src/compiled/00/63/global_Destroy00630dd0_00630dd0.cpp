// _Destroy_range<T*> -- run the virtual destructor (deleting flag 0) over [first, last) (124-byte elements)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00630DD0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je R ; (pad) ; L: mov eax,[esi] ; push 0 ; mov ecx,esi ; call [eax] ; add esi,0x7c ; cmp esi,edi ; jne L ; R: pop edi ; pop esi ; ret
// Dinkumware `_Destroy_range` over 124-byte elements: each element's virtual destructor is called through slot 0 with the deleting flag 0. Fastcall: first in ecx, last in edx. The manifest label is
// propagated; the element type is opaque. One of 5 bodies of this shape.
struct CElement {
    virtual ~CElement();                   // slot 0: the scalar deleting destructor (flag 0: no delete)
    unsigned char _body[0x7c - 4];
};

void __fastcall Destroy_00630dd0(CElement* first, CElement* last)
{
    for (; first != last; ++first)
        first->~CElement();
}
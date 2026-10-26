#pragma optimize("s",on)
// _Uninit_fill_n<CEngineLocalDetailLayerDef*> -- placement-copy `count` copies of value from first (84-byte elements, copy ctor @ 0x004773F1), return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00479C94, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push edi ; mov edi,edx ; L: test esi,esi ; je S ; push [value] ; mov ecx,esi ; call CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef&) ; S: add esi,0x54 ; dec edi ; jne L ; pop edi ; E: mov eax,esi ; pop esi ; ret 4
// Retail std::vector helper (Dinkumware `_Uninit_fill_n`, __fastcall: first in ecx, count in edx,
// the value reference on the stack): copy-constructs `count` elements in place with
// placement new (hence the null test on the destination) through the out-of-line copy
// constructor @ 0x004773F1 and returns the advanced destination. `ret 4` pops the value
// reference. The element type is named from the copy constructor's label when
// unique. One of 26 bodies.
#include <new>

class CEngineLocalDetailLayerDef { public:
    unsigned char _body[0x54];
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);           // 0x004773F1
};

CEngineLocalDetailLayerDef* __fastcall UninitFillN_CEngineLocalDetailLayerDef_00479c94(CEngineLocalDetailLayerDef* first, unsigned long count, const CEngineLocalDetailLayerDef& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CEngineLocalDetailLayerDef(value);
    return first;
}
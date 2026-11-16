// std::__copy<CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*, int> -- copy-assign [first, last) onto dest (count-block release/adopt inlined), return the advanced dest
// Leaf: ??$__copy@PAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535000, oracle length 115 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push ebx ; mov ebx,ecx ; sub edx,ebx ; sar edx,3 ; test edx,edx ; jle Z ; push ebp ; push esi ; mov esi,[dest] ; push edi ; mov [esp+10h],edx ; L: mov edi,[ebx+4] ; mov eax,[esi+4] ; cmp eax,edi ; mov ebp,[ebx] ; je N ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: test edi,edi ; mov [esi],ebp ; mov [esi+4],edi ; je N ; inc dword ptr [edi] ; N: mov eax,[esp+10h] ; add ebx,8 ; add esi,8 ; dec eax ; mov [esp+10h],eax ; jne L ; mov eax,esi ; ... ret 0Ch ; Z: mov eax,[dest] ; ... ret 0Ch
// Retail std::copy back end for CCountedPointer elements (`__fastcall`: first in ecx, last in edx,
// dest and two empty tag arguments on the stack, `ret 0Ch`): the signed element count is kept in
// a stack local and counted down; each assignment is the inlined CCountedPointer::operator= --
// when the count blocks differ, release the destination's block (destroy function @ slot +4
// with the object in ecx, flag-tested operator delete @ 0x00BFE9BC at zero), adopt the source's
// object and block, and bump its counter. Returns the advanced destination (the untouched dest
// for an empty range). One of 14 bodies of this shape.
// Name (round 4, NF40): the STLport random-access __copy instance over CCountedPointer<NUISystem::CComponent> (the landed
//   callers 0x00531EC0 vector<CCountedPointer<CComponent> >::erase and 0x005354E0 already bind this spelling here; Ego_r.pdb
//   holds the Dinkumware twin only, so the name is descriptive-by-library). Was the stand-in Move_CountedPointer_00535000 over
//   local SCountBlock / CCountedPointer / SIteratorTag models; now engine/CCountedPointer.h and the __copy template of the landed
//   0x00B83330 (same body).
#include "engine/CCountedPointer.h"

namespace NUISystem { class CComponent; }

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template CCountedPointer<NUISystem::CComponent>* __fastcall std::__copy<CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*, int>(
    CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*,
    const std::random_access_iterator_tag&, int*);

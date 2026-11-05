// std::__copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, long>(first, last, result, const random_access_iterator_tag&, long*)
// -- STLport 4.6 _algobase.h random-access copy back end: copy-assign [first, last) onto result (count-block release/adopt inlined), return the advanced result
// Leaf: ??$__copy@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@J@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAJ@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Name: chunk 685 NAME_FIXES (the STLport 4.6 instantiation; in neither PDB -- its hand-mangled spelling ...PAV1@0ABU... drops one parameter, VC7.1 emits ...PAV1@00ABU... for the five-parameter text);
//   the row was landed as the descriptive Move_CountedPointer_00ab1480 (naming wave 19: the same body under the template spelling).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1480, oracle length 115 bytes.
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
// for an empty range). One of 12 bodies of this shape.
#include "engine/CCountedPointer.h"

class C3DAnimationBuilder { public: class CBoneInfo; };

namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; 0 < __n; --__n, ++__first, ++__result)
        *__result = *__first;
    return __result;
}
template CCountedPointer<C3DAnimationBuilder::CBoneInfo>* __fastcall __copy<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, long>(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, const random_access_iterator_tag&, long*);
}

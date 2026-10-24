// _global::UninitFillN_CIVCountedPointer_00a6b340(CIVCountedPointer* first, unsigned long count, const CIVCountedPointer& value) -- placement-copy `count` intrusive counted pointers (pointee IVRefCount bumped), return the advanced pointer
// Leaf: UninitFillN_CIVCountedPointer_00a6b340
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B340, 41 bytes. No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; mov ecx,[value] ; push esi ; mov esi,edx ;
//   L: test eax,eax ; je S ; mov edx,[ecx] ; test edx,edx ; mov [eax],edx ; je S ; inc [edx+4] ;
//   S: add eax,4 ; dec esi ; jne L ; pop esi ; E: ret 4
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, the value reference the
// only stack argument -> `ret 4`: this instantiation's caller passes no allocator). The manifest
// label is propagated; the bytes copy a 4-byte CIVCountedPointer whose pointee keeps its
// intrusive count at +4 (engine/CIVCountedPointeeBase.h: vptr +0, IVRefCount +4). The only caller
// is the manifest's Vector_InsertN_DetailPlacementGrid (0x00A6BCD0).
#include <new>
#include "engine/CIVCountedPointeeBase.h"

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

CIVCountedPointer* __fastcall UninitFillN_CIVCountedPointer_00a6b340(CIVCountedPointer* first, unsigned long count, const CIVCountedPointer& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CIVCountedPointer(value);
    return first;
}
// _global::UninitFillN_SPairLongCCompositeSurfaceInfo_00bedbb0(SPairLongCCompositeSurfaceInfo* first, unsigned long count, const SPairLongCCompositeSurfaceInfo& value, CRetailAllocator) -- placement-copy `count` {key, CCompositeSurfaceInfo} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairLongCCompositeSurfaceInfo_00bedbb0
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the non-template local CCountedPointer holds a CCompositeSurfaceCache* (its first member): it is CCountedPointer<CCompositeSurfaceCache>.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEDBB0, 63 bytes (+1 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; mov ecx,[value] ; push esi ; push edi ;
//   lea esi,[eax+8] ; mov edi,edx ; L: test eax,eax ; je S ; [ecx] -> [eax] ;
//   mov [esi-4],vtbl 0x012A8154 ; [ecx+8] -> [esi] ; mov edx,[ecx+0xc] ; test edx,edx ;
//   mov [esi+4],edx ; je S ; inc [edx] ; S: add eax,0x10 ; add esi,0x10 ; dec edi ; jne L ;
//   pop edi ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated: the vtable store is
// RTTI CCompositeSurfaceInfo (0x012A8154), and the 16-byte element is a {4-byte key,
// CCompositeSurfaceInfo} pair. PDB CCompositeSurfaceInfo (0xc): vptr, Cache
// CCountedPointer<CCompositeSurfaceCache> +4 (Data, Info -> CCPPointerInfo::RefCount +0). The
// implicit copy constructor is inlined under the placement-new null test; `&value.second+4` is
// strength-reduced into esi. The only caller is CCompositeSurfaceMap_Reallocate (0x00BEEF90).
#include <new>

class CCompositeSurfaceCache;

#include "engine/CCountedPointer.h"


class CCompositeSurfaceInfo { public:                    // vtable 0x012A8154
    virtual void v00();
    CCountedPointer<CCompositeSurfaceCache> Cache;                        // +4
};

struct SPairLongCCompositeSurfaceInfo {
    unsigned long first;                          // +0
    CCompositeSurfaceInfo second;                 // +4
};

struct CRetailAllocator { };

SPairLongCCompositeSurfaceInfo* __fastcall UninitFillN_SPairLongCCompositeSurfaceInfo_00bedbb0(SPairLongCCompositeSurfaceInfo* first, unsigned long count, const SPairLongCCompositeSurfaceInfo& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongCCompositeSurfaceInfo(value);
    return first;
}
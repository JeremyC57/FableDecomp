// CTCSkeletalMorph::CTCSkeletalMorph(CThing& thing) -- CTCBase(thing), unchanged, empty Morphs array, then PSkeletalMorph takes a new C3DSkeletalMorph
// Leaf: ??0CTCSkeletalMorph@@QAE@AAVCThing@@@Z
// Name fix (NF93, round 8, q11269): the header comment spelled the parameter CThing*; Ego_r.pdb 0x0078DC70 / FableWin.pdb ??0CTCSkeletalMorph@@QAE@AAVCThing@@@Z
//   take CThing& (the code and leaf already did). Comment only.
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCSkeletalMorph@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe 0x007865E0, 96 bytes (`ret 4`). Vtable 0x0126B6F4 = CTCSkeletalMorph (rtti-vtables).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, operator new @ 0x00BFEA1A (0x18 bytes), C3DSkeletalMorph::
// C3DSkeletalMorph() @ 0x00A44E80, CCountedPointer<C3DSkeletalMorph>::Reset(C3DSkeletalMorph*) @ 0x00786BF0 (manifest
// "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", byte twin).
// Layout = Ego_r.pdb CTCSkeletalMorph (0x28): Changed +0x0C, Morphs (CArray<CSkeletalMorph>, retail 12 bytes) +0x10,
// PSkeletalMorph (CCountedPointer<C3DSkeletalMorph>: Data, Info) +0x1C.
// Shape: one shared zero register (ebx) for every init-list zero; the CArray as STLport _Vector_base with the
// _STLP_alloc_proxy end member; the new-expression's null test feeds Reset.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
#include <stddef.h>
void* __cdecl operator new(size_t size);            // 0x00BFEA1A
class CThing;
class C3DSkeletalMorph { public:
    unsigned char _data[0x18];
    C3DSkeletalMorph();                             // 0x00A44E80
};
template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};
template <class T> class CArray { public:                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
#include "engine/CCountedPointer.h"
class CTCSkeletalMorph : public CTCBase { public:                 // vtable 0x0126B6F4
    class CSkeletalMorph;
    bool                              Changed;          // +0x0c
    CArray<CSkeletalMorph>            Morphs;           // +0x10
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;   // +0x1c
    CTCSkeletalMorph(CThing& thing);
};

CTCSkeletalMorph::CTCSkeletalMorph(CThing& thing)
    : CTCBase(thing)
    , Changed(false)
{
    PSkeletalMorph.Reset(new C3DSkeletalMorph);
}
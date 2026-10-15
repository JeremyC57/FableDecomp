#include "rebuild_abi.h"

// sub_00404860(Obj* this) -- trivial constructor: install the vtable (0x0122DC6C) and return this
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404860, 9 bytes. Callees: none.
//
// The whole constructor of a class with a vtable and nothing else to initialise: write the vtable
// pointer and hand the object back. Objects of this shape are the small interface classes we
// derive from; the vtable at 0x0122DC6C identifies which one this is.
//
// Matching: `this` in ecx, moved to eax first so the store and the return share it; the vtable is an
// absolute address, modelled as the address of a global.

extern void* g_vtable_00404860; /* the class's vtable (0x0122DC6C) */

struct Obj_00404860 {
    void* vtbl; /* the only member touched */
};

Obj_00404860* __fastcall sub_00404860(Obj_00404860* thisptr)
{
    *(void**)thisptr = &g_vtable_00404860;
    return thisptr;
}

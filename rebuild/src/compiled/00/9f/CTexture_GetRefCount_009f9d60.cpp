// CTexture::GetRefCount  retail 0x009f9d60
// Leaf: ?GetRefCount@CTexture@@QAEJXZ
// member 0 = pointer to a COM-like interface object whose [0] is a vtable of
// stdcall function pointers taking the object pointer explicitly.
// slot1 ([vtbl+4]) called for side effect; slot2 ([vtbl+8]) returns the count.

// Name (NF378): Ego_r.pdb / FableWin.pdb ?GetRefCount@CTexture@@QAEJXZ; BSim 1.0 and Ego_r lib_texture.obj order (was the stand-in
// CTexture_Methods). Declared by the engine/CTexture.h opt-in below.
#define FABLE_RETAIL_CTEXTURE_GETREFCOUNT
#include "engine/CTexture.h"  // retyped onto the PDB layout; byte parity re-verified
struct IDirect3DTexture9;
struct IRefVtbl {
    long (__stdcall *slot0)(IDirect3DTexture9*);
    long (__stdcall *slot1)(IDirect3DTexture9*);
    long (__stdcall *slot2)(IDirect3DTexture9*);
};
struct IDirect3DTexture9 {
    IRefVtbl* vtbl;
};

long CTexture::GetRefCount()
{
    IDirect3DTexture9* p = this->PD3DTexture;
    if (p != 0)
    {
        p->vtbl->slot1(p);
        p = this->PD3DTexture;
        return p->vtbl->slot2(p);
    }
    return 0;
}
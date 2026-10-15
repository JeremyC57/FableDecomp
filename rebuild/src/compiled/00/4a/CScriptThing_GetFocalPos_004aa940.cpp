// CScriptThing::GetFocalPos() -- the pointee's FocalPos, or a copy of the static zero vector when there is no pointee
// Leaf: GetFocalPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA940, length 57 bytes (`?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ`, a const virtual).
// Callees: virtual slot 5 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp_Data (+0x04) through its vtable. engine/CScriptThing.h gives the
// handle layout; the pointee's vtable is reproduced through a local slot view cast from PImp_Data
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested. Null arm: the inline memberwise
// copy constructor of the global (x through the x87, y/z through integer registers) into the
// hidden result, no frame; else `push esi` of the hidden pointer and the virtual (slot 5,
// `call [eax+14h]`), returning that pointer.
#include "engine/CScriptThing.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy: hidden-pointer return
};
extern C3DVector g_ZeroVector;                      // 0x0143E8E0 (12 bytes, directly before the empty string)

struct CScriptThingVirtuals {                       // vtable view of the implementation object
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual C3DVector GetFocalPos() const;                 // slot 5 (+0x14)
};

struct CScriptThing_Methods : CScriptThing {
    C3DVector GetFocalPos() const;
};

C3DVector CScriptThing_Methods::GetFocalPos() const
{
    const CScriptThingVirtuals* imp = (const CScriptThingVirtuals*)PImp_Data;
    if (imp == 0)
        return g_ZeroVector;
    return imp->GetFocalPos();
}
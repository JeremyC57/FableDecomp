// CScriptThing::GetHomePos() -- the pointee's HomePos, or a copy of the static zero vector when there is no pointee
// Leaf: GetHomePos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA9A0, length 57 bytes (`?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ`, a const virtual).
// Callees: virtual slot 7 of the pointee.
// CScriptThing is a pimpl handle {vptr, CCountedPointer<CScriptThing> PImp}: every query forwards to the
// implementation object at PImp_Data (+0x04) through its vtable. engine/CScriptThing.h gives the
// handle layout; the pointee's vtable is reproduced through a local slot view cast from PImp_Data
// (the landed GetPos 0x004AA980 / GetCurrentMapName 0x004AA9E0 siblings use the same model).
// Shape: `PImp_Data` loaded straight into ecx and null-tested. Null arm: the inline memberwise
// copy constructor of the global (x through the x87, y/z through integer registers) into the
// hidden result, no frame; else `push esi` of the hidden pointer and the virtual (slot 7,
// `call [eax+1Ch]`), returning that pointer.
#include "engine/CScriptThing.h"

class C3DVector { public:
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
    virtual void v5();
    virtual void v6();
    virtual C3DVector GetHomePos() const;                 // slot 7 (+0x1c)
};

struct CScriptThing_Methods : CScriptThing {
    C3DVector GetHomePos() const;
};

C3DVector CScriptThing_Methods::GetHomePos() const
{
    const CScriptThingVirtuals* imp = (const CScriptThingVirtuals*)PImp_Data;
    if (imp == 0)
        return g_ZeroVector;
    return imp->GetHomePos();
}
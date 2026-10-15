// CScriptThing::GetAngleXY() -- forward the XY heading to the wrapped implementation, 0.0f when the handle is null
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AAA40, 20 bytes (`?GetAngleXY@CScriptThing@@UBEMXZ`, vtable 0x01238C8C slot 10).
// Callees: the implementation object's own GetAngleXY (virtual slot 10 = +0x28); 0.0f comes from the
// float pool at 0x0122DEDC.
//
// Shape: `mov ecx,[ecx+4] ; test ; jne ; fld [0.0f] ; ret ; mov eax,[ecx] ; call [eax+28h] ; ret`.
// CScriptThing (PDB: 0xc bytes, vptr + CCountedPointer<CScriptThing> PImp {Data +4, Info +8}) is a
// pimpl handle: every virtual of the handle forwards to the SAME virtual of the CScriptThing held in
// PImp.Data. The engine header (engine/CScriptThing.h) supplies the PImp_Data member; the handle's
// virtual table is modelled on the implementation side (slot 10 = GetAngleXY, per rtti-vtables.tsv).
// The float result goes through a named local: VC7.1 sibling-calls (`jmp`) a float-returning
// forwarder otherwise, where retail kept `call ; ret`.
// Leaf: GetAngleXY
#include "engine/CScriptThing.h"

// The object behind PImp_Data is itself a CScriptThing (the implementation subclass); only the
// slot this body reaches is named, the others hold its place in the table.
struct CScriptThingImplementation {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual float GetAngleXY() const;                     // slot 10 (+0x28)
};

struct CScriptThing_Methods : CScriptThing {
    float GetAngleXY() const;
};

float CScriptThing_Methods::GetAngleXY() const
{
    const CScriptThingImplementation* implementation =
        reinterpret_cast<const CScriptThingImplementation*>(PImp_Data);
    if (implementation == 0)
        return 0.0f;
    float angle = implementation->GetAngleXY();
    return angle;
}
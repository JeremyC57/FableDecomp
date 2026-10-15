// _unattributed::CanBlockNavigatingCreatureFilter(CThingPhysical* thing) -- thing filter: ask the physics component's slot-42 flag, then return the thing's CanBlockNavigatingCreature() on either path
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050EEF0, 52 bytes. Manifest label "CScriptThing::IsAlive" is propagated
// (the real CScriptThing::IsAlive is 0x004AB130) and rejected: `this` is never read and the argument is a
// thing whose +0x60 is the physics component. Callers: two GetAllThings/CFilter_Bind2 instances
// (0x00823738, 0x0082673B) call it as the SECOND bound filter (`mov ecx,[filter+4] ; push thing`), the first
// being CThingFilter_BoundingBoxOverlapsMap::operator() 0x006A7FB0 -- so this is a filter class's
// operator()(CThing*) const whose class name is not recoverable from the bytes.
// Callees: CTCPhysicsBase slot 42 (+0xA8; 0x006B1100 returns false, CTCPhysicsStandard 0x00724BC0 returns
// bit 4 of +0x85) and CThingPhysical::CanBlockNavigatingCreature (thing slot 55, +0xDC, 0x006A4D90).
//
// Shape: both arms end in the same `call [vtbl+0DCh] ; pop esi ; ret 4` -- VC7.1 keeps two explicit
// `return thing->CanBlockNavigatingCreature();` statements apart (no tail merge). The component pointer
// is a named local so it is loaded straight into ecx (`mov ecx,[esi+60h] ; test ecx,ecx`); reading the
// member twice loads eax and copies it. Retail CThing sits 0x10 below the PDB layout here (PhysicsTC PDB
// +0x70 -> retail +0x60, ThingTypeFlags +0x7C -> +0x6C), so the members are modelled locally.
// Leaf: CanBlockNavigatingCreatureFilter

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41();
    virtual bool IsPhysicsFlagSet();         // slot 42 (+0xA8)
};

struct CThingPhysical {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual bool CanBlockNavigatingCreature();   // slot 55 (+0xDC), 0x006A4D90
    unsigned char _pad_0x04[0x5c];
    CTCPhysicsBase* PhysicsTC;                   // +0x60
};

struct CThingFilter_0050EEF0 {
    bool CanBlockNavigatingCreatureFilter(CThingPhysical* thing) const;
};

bool CThingFilter_0050EEF0::CanBlockNavigatingCreatureFilter(CThingPhysical* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && physics->IsPhysicsFlagSet())
        return thing->CanBlockNavigatingCreature();
    return thing->CanBlockNavigatingCreature();
}
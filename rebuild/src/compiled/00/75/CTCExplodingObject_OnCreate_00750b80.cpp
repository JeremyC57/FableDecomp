// CTCExplodingObject::OnCreate() -- when the object explodes on creature proximity, ask its thing to be updated every frame
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750B80, length 18 bytes (vtable slot 4, OnCreate; the manifest's vfunc_4).
// Callees: CThing::SetForceFrameUpdate(bool) @ 0x004C7C60 (sets bit 3 of the thing's flag byte at +0x92).
// Layout: PDB CTCExplodingObject (48 bytes) over CTCBase (engine/CTCBase.h): MaxDamage +0xc, Radius +0x10,
// FireDamage +0x14, CountdownTimer +0x18, TriggeredOnCreatureProximity +0x1c, TriggerRadius +0x20, PDef +0x24,
// PProximityParticle +0x28.
// Shape: a bool member test (`mov al,[ecx+1Ch] ; test al,al ; je`) guarding a call on ParentThing with `true`; a
// void call that is not the last statement of a two-way body keeps `call ; ret` (no tail jump with an argument push).
#include "engine/CTCBase.h"

struct CThing {
    void SetForceFrameUpdate(bool update);         // 0x004C7C60
};

struct CTCExplodingObject : CTCBase {
    float MaxDamage;                               // +0x0c
    float Radius;                                  // +0x10
    long  FireDamage;                              // +0x14
    long  CountdownTimer;                          // +0x18
    bool  TriggeredOnCreatureProximity;            // +0x1c
    float TriggerRadius;                           // +0x20
    void OnCreate();
};

void CTCExplodingObject::OnCreate()
{
    if (TriggeredOnCreatureProximity)
        ParentThing->SetForceFrameUpdate(true);
}
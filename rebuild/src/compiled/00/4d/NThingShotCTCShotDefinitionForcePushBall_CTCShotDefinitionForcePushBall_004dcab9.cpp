#pragma optimize("s", on)
// NThingShot::CTCShotDefinitionForcePushBall::CTCShotDefinitionForcePushBall(CThing*) -- base CTCShotDefinitionProjectile(thing), DistanceToPush zeroed, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCAB9, length 29 bytes.
// Callees: CTCShotDefinitionProjectile::CTCShotDefinitionProjectile(CThing*) @ 0x004DC991. Vtable 0x01240FAC = NThingShot::CTCShotDefinitionForcePushBall
// (rtti-vtables.tsv); the manifest label "CCrimeStatBar::CCrimeStatBar" is propagated from a same-shape body.
// Shape: the size-optimised TU (odd address in the /Os region) forwards the argument with `push dword ptr [esp+8]` and
// stores the float zero through the x87 (`fldz ; fstp`). PDB CTCShotDefinitionForcePushBall (172 bytes) puts
// DistanceToPush at +0xa8; retail stores it at +0xa4 (the base is 4 bytes smaller in the shipped build).
struct CThing;

struct CTCShotDefinitionProjectile {
    virtual void v00();
    unsigned char _body[0xa4 - 4];             // +0x04 .. +0xa4: the projectile definition state
    CTCShotDefinitionProjectile(CThing* thing);            // 0x004DC991
};

namespace NThingShot {

struct CTCShotDefinitionForcePushBall : CTCShotDefinitionProjectile {   // vtable 0x01240FAC
    virtual void v00();
    float DistanceToPush;                      // +0xa4 (PDB +0xa8)
    CTCShotDefinitionForcePushBall(CThing* thing);
};

CTCShotDefinitionForcePushBall::CTCShotDefinitionForcePushBall(CThing* thing)
    : CTCShotDefinitionProjectile(thing), DistanceToPush(0.0f)
{
}

}
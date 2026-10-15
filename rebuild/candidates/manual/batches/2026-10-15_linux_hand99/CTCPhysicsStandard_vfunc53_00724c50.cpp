// CTCPhysicsStandard::vfunc_53() -- true when the body is OnGround or OnMesh (bits 4 / 5 of the flag byte at +0x84)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724C50, length 20 bytes (vtable 0x01265F84 slot 53, rtti-vtables.tsv).
// Callees: none.
// Shape: `movzx ecx,byte [ecx+84h] ; mov eax,ecx ; shr eax,1 ; or eax,ecx ; shr eax,4 ; and eax,1` -- the two bitfields are
// combined with a bitwise `|` (no branch), so the byte is widened once and both bits fold into one shift/or/mask.
// Layout: Ego_r.pdb CTCPhysicsStandard flags byte +0x84 (AlwaysFacingMovementDirection .. CollidesWithThings, `bool : 1`).
// engine/CTCPhysicsStandard.h keeps the byte opaque and has no virtuals, so the class is modelled locally.
struct CTCPhysicsStandard {
    virtual void v00();
    unsigned char _base[0x80];                 // +0x04 .. +0x83
    bool AlwaysFacingMovementDirection : 1;    // +0x84 bit 0
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    bool OnGround : 1;                         // bit 4
    bool OnMesh : 1;                           // bit 5
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    bool vfunc_53();
};

bool CTCPhysicsStandard::vfunc_53()
{
    return OnGround | OnMesh;
}

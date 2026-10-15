// CTCPhysicsStandard::vfunc_84(bool) -- sets the PushableByHero flag (bit 5 of the flag byte at +0x85)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724D00, length 28 bytes (vtable 0x01265F84 slot 84, rtti-vtables.tsv).
// Callees: none.
// Shape: `mov al,[esp+4] ; mov dl,[ecx+85h] ; shl al,5 ; xor al,dl ; and al,20h ; xor dl,al` -- the VC7.1 one-bit bitfield
// store from a `bool` argument (xor-merge into the old byte).
// Layout: Ego_r.pdb CTCPhysicsStandard flags byte +0x85 (CollidesWithThingsOneWay .. Impassable). engine/CTCPhysicsStandard.h
// keeps the byte opaque and has no virtuals, so the class is modelled locally.
struct CTCPhysicsStandard {
    virtual void v00();
    unsigned char _base[0x80];                 // +0x04 .. +0x83
    unsigned char _flags84;                    // +0x84 AlwaysFacingMovementDirection .. CollidesWithThings
    bool CollidesWithThingsOneWay : 1;         // +0x85 bit 0
    bool CollidesWithLandscape : 1;
    bool PhysicsEnabled : 1;
    bool Immoveable : 1;
    bool NavigationLayers : 1;
    bool PushableByHero : 1;                   // bit 5
    bool ExcludingCreatures : 1;
    bool Impassable : 1;
    void vfunc_84(bool value);
};

void CTCPhysicsStandard::vfunc_84(bool value)
{
    PushableByHero = value;
}

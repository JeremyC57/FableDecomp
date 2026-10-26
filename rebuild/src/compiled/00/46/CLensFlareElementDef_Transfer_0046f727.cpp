// CLensFlareElementDef::Transfer(CPersistContext& context) -- persist one lens-flare element def: radius, texture, position along the flare axis and colour
// Leaf: ?Transfer@CLensFlareElementDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046F727, 57 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers, catalog labels propagated): float 0x00431061, long 0x00431102,
//   CRGBColour 0x004595A3 (the instantiation the landed CPlayerDef::Transfer 0x00459582 calls).
// Layout: Ego_r.pdb CLensFlareElementDef (0x50) re-based on the retail bytes: the def base ends at +0x3c (PDB +0x40), so
//   Radius +0x3c, Colour +0x40, Position +0x44, Texture +0x48. No generated engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in the def's TRANSFER list order (Radius, Texture, Position, Colour); the last
//   member's address is the natural `add esi,40h ; push esi`.
#pragma optimize("s", on)

class CRGBColour { public: unsigned char B, G, R, A; };

class CPersistContext {
public:
    template <class T> void Transfer(T& value);                // 0x00431061
};

class CLensFlareElementDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                  // CDefClassBase data (+0x04..+0x3b)
    float Radius;                               // +0x3c
    CRGBColour Colour;                          // +0x40
    float Position;                             // +0x44
    long Texture;                               // +0x48
};

void CLensFlareElementDef::Transfer(CPersistContext& context)
{
    context.Transfer(Radius);
    context.Transfer(Texture);
    context.Transfer(Position);
    context.Transfer(Colour);
}
// CThing::vfunc_29() -- CThing vtable slot 29: the same drawable test as DrawIsDrawable (Drawable flag set, activated, not dead), a separate virtual (the drawable-in-editor query)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C77B0, 32 bytes. CThing vtable 0x01239ABC slot 29 (byte-identical to slot 27 0x004C7790). No callees.
// Shape: Drawable is bit 1 of +0x92, Activated bit 3 and Dead bit 0 of +0x91 (retail offsets, PDB +0x10 higher);
// `return a && b && !c;` materialises the bool through the full register (`mov eax,1` / `xor eax,eax`).
class CThing { public:
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                          // +0x91 bit 0 (PDB +0xa1)
    unsigned char CreatedByOwnerThingCreation : 1;   // bit 1
    unsigned char IncludeInThingScans : 1;           // bit 2
    unsigned char Activated : 1;                     // bit 3
    unsigned char GloballyLoaded : 1;                // bit 4
    unsigned char InLimbo : 1;                       // bit 5
    unsigned char KillLocked : 1;                    // bit 6
    unsigned char CreatedBySerialisation : 1;        // bit 7
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; // +0x92 bit 0
    unsigned char Drawable : 1;                      // bit 1
    bool vfunc_29();
};

bool CThing::vfunc_29()
{
    return Drawable && Activated && !Dead;
}
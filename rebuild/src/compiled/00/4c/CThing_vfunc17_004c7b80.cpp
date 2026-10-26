// CThing::vfunc_17() -- CThing vtable slot 17: true when the thing has a base def and is included in thing scans, activated, alive, not in limbo, and its def's flag (bit 7 of def +0x41, InMapSearches) is set
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7B80, 53 bytes. CThing vtable 0x01239ABC slot 17. No callees.
// Shape: PBaseDef is retail +0x70 (PDB +0x80); the def byte +0x41 bit 7 is tested with `test al,al ; jns`.
// IncludeInThingScans/Activated/Dead/InLimbo are bits 2/3/0/5 of +0x91. The early `xor al,al` (no def) is
// a plain `return false`, the rest one `&&` chain (full-register 0/1). `!Dead` goes through an inline
// `IsAlive()` returning bool: retail materialises it (`mov cl,al ; not cl ; test cl,1`); the direct bit test
// merges Dead and InLimbo into one `test cl,21h`.
class CThingBaseDef { public:
    unsigned char _pad00[0x41];
    unsigned char AvailableInEditor : 1;             // +0x41 bit 0
    unsigned char AlwaysFrameUpdate : 1;             // bit 1
    unsigned char StartKillLocked : 1;               // bit 2
    unsigned char LevelLess : 1;                     // bit 3
    unsigned char IsDrawable : 1;                    // bit 4
    unsigned char IsDrawableInEditor : 1;            // bit 5
    unsigned char ForceNoSerialise : 1;              // bit 6
    unsigned char InMapSearches : 1;                 // bit 7
};

class CThing { public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x70 - 0x04];
    CThingBaseDef*  PBaseDef;                        // +0x70 CDefPointer<CThingBaseDef const> (PDB +0x80)
    unsigned char   _pad74[0x91 - 0x74];
    unsigned char   Dead : 1;                        // +0x91 bit 0 (PDB +0xa1)
    unsigned char   CreatedByOwnerThingCreation : 1; // bit 1
    unsigned char   IncludeInThingScans : 1;         // bit 2
    unsigned char   Activated : 1;                   // bit 3
    unsigned char   GloballyLoaded : 1;              // bit 4
    unsigned char   InLimbo : 1;                     // bit 5
    bool IsAlive() const { return !Dead; }
    bool vfunc_17();
};

bool CThing::vfunc_17()
{
    CThingBaseDef* def = PBaseDef;
    if (!def)
        return false;
    return IncludeInThingScans && Activated && IsAlive() && !InLimbo && def->InMapSearches;
}
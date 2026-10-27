// CThing::DriverQuery_004c7870() -- asks the thing's driver component (its first own virtual, slot 23) and answers false when the thing has no driver
// Grade: DIFFER(15v15) -- retail preloads `xor al,al` before `test ecx,ecx` (falls through to the tail jump); ours puts `xor al,al` after the jump at the end; tried if/return, ternary, both under optimize("s")
// Leaf: ?DriverQuery_004c7870@CThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7870, 15 bytes: mov ecx,[ecx+5Ch] ; xor al,al ; test ecx,ecx ; je ret ;
//   mov eax,[ecx] ; jmp [eax+5Ch] ; ret.
// Identity: CThing vtable 0x01239ABC slot 22 (also in six derived-thing vtables); the catalog "CThing::vfunc_22" is the
//   slot name. Descriptive name: no PDB name set gives CThing's slot order. The callee is CTCDriverBase's first own
//   virtual (slot 23, +0x5C, after CTCBase's GetInterfaceType 21 / GetClassTypeName 22): CTCDriverBase answers false
//   (0x00686970, `xor al,al`), CTCDCameraPoint / CTCDNavigationSeed / CTCDRegionEntrance / CTCDRegionExit answer true and
//   CTCDParticleEmitter returns bit 2 of its +0x44 flags.
// Layout: retail CThing PTCDriver +0x5C (Ego_r.pdb +0x6C; landed 0x007949F0 and others). No engine header for CThing.
// Shape: one ternary (`driver ? driver->DriverQuery() : false`): VC7.1 preloads `false` into al before the null test
//   (the if/return form puts `xor al,al` after the jump) and the call is a tail jump through the driver's vtable.
class CTCDriverBase {
public:
    virtual ~CTCDriverBase();                   // slot 0
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05(); virtual void _v06(); virtual void _v07();
    virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14();
    virtual void _v15(); virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21();
    virtual void _v22();
    virtual bool DriverQuery() const;           // slot 23 (+0x5C), descriptive
};

class CThing {
public:
    virtual ~CThing();                          // slot 0
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05(); virtual void _v06(); virtual void _v07();
    virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14();
    virtual void _v15(); virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21();
    virtual bool DriverQuery_004c7870() const;  // slot 22 (+0x58)
    unsigned char  _pad004[0x5c - 0x4];
    CTCDriverBase* PTCDriver;                   // +0x5C
};

bool CThing::DriverQuery_004c7870() const
{
    CTCDriverBase* driver = PTCDriver;
    return driver ? driver->DriverQuery() : false;
}

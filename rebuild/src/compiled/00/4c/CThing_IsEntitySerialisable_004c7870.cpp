// CThing::IsEntitySerialisable() const -- a thing is serialised when its driver component says so; no driver, no save
// Leaf: ?IsEntitySerialisable@CThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7870, 15 bytes: mov ecx,[ecx+5Ch] ; xor al,al ; test ecx,ecx ; je ret ;
//   mov eax,[ecx] ; jmp [eax+5Ch] ; ret.
// Identity (proven): CThing vtable 0x01239ABC slot 22 (+0x58). The landed CThingBuilding_IsEntitySerialisable_0082d0c0
//   proves slot 22 = ?IsEntitySerialisable@<Class>@@UBE_NXZ (the eleven classes overriding slot 22 are exactly the
//   PDB's IsEntitySerialisable class set); Ego_r.pdb names ?IsEntitySerialisable@CThing@@UBE_NXZ. Label correction:
//   catalog "CThing::vfunc_22".
// Callee: the driver's slot 23 (+0x5C) virtual = CTCBase::IsComponentSerialisable() const (PDB UBE_NXZ). The
//   overrides fit the PDB set: CTCDCameraPoint / CTCDNavigationSeed / CTCDRegionEntrance / CTCDRegionExit answer
//   true, CTCDParticleEmitter tests a flag bit, all four PDB-named ?IsComponentSerialisable@<Class>@@UBE_NXZ.
// Layout: retail CThing PTCDriver +0x5C (Ego_r.pdb +0x6C; Dinkumware vs STLport container sizes, landed 0x007949F0).
//   No engine header for CThing; CTCBase.h flattens the vptr, so a local polymorphic view is used.
// Shape: a `bool result = false;` local assigned under the null test: VC7.1 preloads `xor al,al` before `test ecx`
//   and turns the call into a tail jump. The ternary / if-return forms put `xor al,al` after the jump.
class CTCDriverBase {
public:
    virtual ~CTCDriverBase();                   // slot 0
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05(); virtual void _v06(); virtual void _v07();
    virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14();
    virtual void _v15(); virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21();
    virtual void _v22();
    virtual bool IsComponentSerialisable() const;   // slot 23 (+0x5C)
};

class CThing {
public:
    virtual ~CThing();                          // slot 0
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05(); virtual void _v06(); virtual void _v07();
    virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14();
    virtual void _v15(); virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21();
    virtual bool IsEntitySerialisable() const;  // slot 22 (+0x58)
    unsigned char  _pad004[0x5c - 0x4];
    CTCDriverBase* PTCDriver;                   // +0x5C
};

bool CThing::IsEntitySerialisable() const
{
    bool result = false;
    CTCDriverBase* driver = PTCDriver;
    if (driver)
        result = driver->IsComponentSerialisable();
    return result;
}
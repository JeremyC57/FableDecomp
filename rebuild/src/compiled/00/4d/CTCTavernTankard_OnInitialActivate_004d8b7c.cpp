// CTCTavernTankard::OnInitialActivate() -- empties the tankard (Fullness = 0.0f)
// Leaf: ?OnInitialActivate@CTCTavernTankard@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8B7C, length 6 bytes, plain `ret`. CTCTavernTankard vtable 0x0123EDCC
// slot 6, the CTCBase OnInitialActivate slot (Ego_r.pdb ?OnInitialActivate@CTCTavernTankard@@UAEXXZ; catalog "vfunc_6").
// Layout: Ego_r.pdb CTCTavernTankard (0x3C bytes): CTCBase (0x0C) then Fullness +0x0C (float). No engine header.
// Shape: `fldz ; fstp dword ptr [ecx+0Ch]`; the packed, unaligned address marks a size-optimised TU.
#pragma optimize("s", on)
class CThing;

class CTCTavernTankard {
public:
    virtual ~CTCTavernTankard();                // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void OnInitialActivate();           // slot 6 (+0x18)
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08 (CTCBase)
    bool    IsInGlobalUpdateTCs;                // +0x09 (CTCBase)
    float   Fullness;                           // +0x0C
};

void CTCTavernTankard::OnInitialActivate()
{
    Fullness = 0.0f;
}
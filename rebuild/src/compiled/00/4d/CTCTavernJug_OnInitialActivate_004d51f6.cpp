// CTCTavernJug::OnInitialActivate() -- empties the jug and puts it back on the bar (Fullness = 0, OnBar = true)
// Leaf: ?OnInitialActivate@CTCTavernJug@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D51F6, length 9 bytes, plain `ret`. CTCTavernJug vtable 0x0123CA14
// slot 6, the CTCBase OnInitialActivate slot (Ego_r.pdb ?OnInitialActivate@CTCTavernJug@@UAEXXZ; catalog "vfunc_6").
// Layout: Ego_r.pdb CTCTavernJug (0x38 bytes): CTCBase (vptr, ParentThing, ToBeRemoved/IsInGlobalUpdateTCs) then
// Fullness +0x0C (int), OnBar +0x10 (bool). No engine header for the class.
// Shape: the packed, unaligned address and `and dword ptr [ecx+0Ch],0` (a 4-byte zero store) mark a size-optimised TU.
#pragma optimize("s", on)
class CThing;

class CTCTavernJug {
public:
    virtual ~CTCTavernJug();                    // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void OnInitialActivate();           // slot 6 (+0x18)
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08 (CTCBase)
    bool    IsInGlobalUpdateTCs;                // +0x09 (CTCBase)
    int     Fullness;                           // +0x0C
    bool    OnBar;                              // +0x10
};

void CTCTavernJug::OnInitialActivate()
{
    Fullness = 0;
    OnBar = true;
}
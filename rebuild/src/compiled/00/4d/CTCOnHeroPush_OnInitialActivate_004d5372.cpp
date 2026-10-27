// CTCOnHeroPush::OnInitialActivate() -- resets the push state: FrameSincePushed = 0, DistPushed = 0.0f
// Leaf: ?OnInitialActivate@CTCOnHeroPush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5372, length 10 bytes, plain `ret`. CTCOnHeroPush vtable 0x0123CBC4
// slot 6, the CTCBase OnInitialActivate slot (Ego_r.pdb ?OnInitialActivate@CTCOnHeroPush@@UAEXXZ; catalog "vfunc_6").
// Layout: Ego_r.pdb CTCOnHeroPush (0x20 bytes): CTCBase (0x0C) then DistPushed +0x0C (float), PushDir +0x10 (C3DVector),
// FrameSincePushed +0x1C (long). No engine header for the class.
// Shape: size-optimised TU (packed address, `and dword ptr [ecx+1Ch],0`); the stores follow source order (the frame
// counter first, then `fldz ; fstp` for the float).
#pragma optimize("s", on)
class CThing;

class C3DVector { public: float X, Y, Z; };

class CTCOnHeroPush {
public:
    virtual ~CTCOnHeroPush();                   // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void OnInitialActivate();           // slot 6 (+0x18)
    CThing*   ParentThing;                      // +0x04 (CTCBase)
    bool      ToBeRemoved;                      // +0x08 (CTCBase)
    bool      IsInGlobalUpdateTCs;              // +0x09 (CTCBase)
    float     DistPushed;                       // +0x0C
    C3DVector PushDir;                          // +0x10
    long      FrameSincePushed;                 // +0x1C
};

void CTCOnHeroPush::OnInitialActivate()
{
    FrameSincePushed = 0;
    DistPushed = 0.0f;
}